//
// Created by joaop on 25/11/2025.
//

#include "entities.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <cjson/cJSON.h>
#include <string.h>
#include "binService.h"

int get_page_title_item(FILE* fp, TitlesResponse *r) {
    char *buffer  = read_entire_file(fp);
    int pageCount = 0;
    fseek(fp, 0, SEEK_SET);
    //-----parse buffer--------------------
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL) {
        const char *error = cJSON_GetErrorPtr();
        if (error) {
            printf("JSON Parse Error before: %s\n", error);
        }
        return 0;
    }
    cJSON *totalCount = cJSON_GetObjectItem(root, "totalCount");
    r->totalCount = totalCount ? totalCount->valueint : 0; //analyse cJSON* totalCount and write valueInt or 0
    cJSON *nextPageToken = cJSON_GetObjectItem(root, "nextPageToken");
    if (!nextPageToken) {
        r->token = NULL;
    }
    else {
        r->token = json_strdup(nextPageToken);
    }

    cJSON *titles = cJSON_GetObjectItem(root, "titles");
    if (!titles || !cJSON_IsArray(titles)) {
        cJSON_Delete(root);
        free(buffer);
        return 0;
    }
    pageCount = cJSON_GetArraySize(titles);
    r->pageCount = pageCount;
    r->titles = malloc(sizeof(ParseTitle) * pageCount);
    for (int i = 0; i < pageCount; i++) {
        r->titles[i] = parse_title(cJSON_GetArrayItem(titles, i));
    }
    cJSON_Delete(root);
    free(buffer);
    return pageCount;
}

ParseTitle parse_title(const cJSON *item) {
    ParseTitle t = {0};

    t.id = json_strdup(cJSON_GetObjectItem(item, "id"));
    t.type = json_strdup(cJSON_GetObjectItem(item, "type"));
    t.primaryTitle = json_strdup(cJSON_GetObjectItem(item, "primaryTitle"));
    t.originalTitle = json_strdup(cJSON_GetObjectItem(item, "originalTitle"));
    t.plot = json_strdup(cJSON_GetObjectItem(item, "plot"));

    cJSON *startYear = cJSON_GetObjectItem(item, "startYear");
    if (cJSON_IsNumber(startYear)) t.startYear = startYear->valueint;

    cJSON *runtime = cJSON_GetObjectItem(item, "runtimeSeconds");
    if (cJSON_IsNumber(runtime)) t.runtimeSeconds = runtime->valueint;

    // ---- primaryImage ---- wont be used
    /*    cJSON *image = cJSON_GetObjectItem(item, "primaryImage");
        if (cJSON_IsObject(image)) {
            t.image.href = json_strdup(cJSON_GetObjectItem(image, "url"));
            t.image.width = cJSON_GetObjectItem(image, "width")->valueint;
            t.image.height = cJSON_GetObjectItem(image, "height")->valueint;
        }
    */
    // ---- rating ----
    cJSON *rating = cJSON_GetObjectItem(item, "rating");
    if (cJSON_IsObject(rating)) {
        t.rating.aggregateRating = cJSON_GetObjectItem(rating, "aggregateRating")->valuedouble;
        t.rating.voteCount = cJSON_GetObjectItem(rating, "voteCount")->valueint;
    }

    // ---- genres array ----
    cJSON *genres = cJSON_GetObjectItem(item, "genres");
    if (cJSON_IsArray(genres)) {
        t.genres_count = cJSON_GetArraySize(genres);
        t.genres = malloc(sizeof(char*) * t.genres_count);
        for (int i = 0; i < t.genres_count; i++) {
            cJSON *g = cJSON_GetArrayItem(genres, i);
            t.genres[i] = json_strdup(g);
        }
    }

    return t;
}
static char* json_strdup(const cJSON* obj) {
    return (obj && cJSON_IsString(obj)) ? strdup(obj->valuestring) : NULL;
}

void free_titles_response(TitlesResponse *r) {
    for (int i = 0; i < r->pageCount; i++) {
        ParseTitle *t = &r->titles[i];

        free(t->id);
        free(t->type);
        free(t->primaryTitle);
        free(t->originalTitle);
        free(t->plot);

        for (int g = 0; g < t->genres_count; g++)
            free(t->genres[g]);

        free(t->genres);
    }
    free(r->titles);
    free(r->token);
    free(r);
}



char *read_entire_file(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buf = malloc(size + 1);
    if (!buf) return NULL;

    size_t n = fread(buf, 1, size, fp);
    buf[n] = '\0';
    return buf;
}


void record_titles_page_on_binary(const ParseTitle* titlesArray, FileHeader fHeader, int pageCount, FILE* fp) {
    for (int i = 0; i < pageCount; i++) {
        Titles entry = {0};
        if (!titlesArray[i].plot) {
            printf("NULL plot detected at index %d\n", i);
        }
        entry.id = i + fHeader.recordCount;
        put_title(entry, titlesArray[i], &fHeader, fp);
    }
}

