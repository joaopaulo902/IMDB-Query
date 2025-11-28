//
// Created by joaop on 28/11/2025.
//

#include "dbContext.h"
#include "entities.h"
#include "apiHandler.h"
#include <string.h>

void make_titles_full_request() {
    char url[1024] = {IMDB_QUERY_URL};
    int i = 0;
    FileHeader fH = {0};

    FILE* binFp = fopen("titles.bin", "rb+");
    if (!binFp)
        binFp = fopen("titles.bin", "wb+");

    fseek(binFp, 0, SEEK_SET);

    if (is_file_empty(binFp)) {
        fH.ID = TITLE_FILE_ID;
        fH.version = 1;
        fH.recordCount = 0;
        fH.nextPageToken[0] = '\0';

        fwrite(&fH, sizeof(FileHeader), 1, binFp);
        fseek(binFp, sizeof(FileHeader), SEEK_SET);
    }
    else {
        if (fread(&fH, sizeof(FileHeader), 1, binFp) != 1) {
            perror("Failed to read header");
            fclose(binFp);
            return;
        }

        if (fH.ID != TITLE_FILE_ID) {
            printf("Invalid file format\n");
            fclose(binFp);
            return;
        }

        if (fH.nextPageToken[0] != '\0') {
            snprintf(url, sizeof(url), "%s?pageToken=%s", IMDB_QUERY_URL, fH.nextPageToken);
            fseek(binFp, (long) (sizeof(FileHeader) + sizeof(Titles) * fH.recordCount), SEEK_SET );
        }
    }
    printf("%llu\n", fH.recordCount);
    do {
        printf("%d\n", i++);
        TitlesResponse* t = malloc(sizeof(TitlesResponse));
        FILE* filePointer = fopen("data.json", "w");
        if (!filePointer) {
            perror("Failed to open data.json on writing");
            free_titles_response(t);
            break;
        }
        get_info(url, filePointer);
        fclose(filePointer);
        FILE* jsonFilePointer = fopen("data.json", "r");
        if (!jsonFilePointer) {
            perror("Failed to open data.json on reading");
            free_titles_response(t);
            break;
        }
        int pageCount = get_page_title_item(jsonFilePointer, t);
        fclose(jsonFilePointer);
        record_titles_page_on_binary(t->titles, fH, pageCount , binFp);
        fH.recordCount += pageCount;
        if (t->token != NULL && strlen(t->token) > 0) {
            snprintf(url, sizeof(url), "%s?pageToken=%s", IMDB_QUERY_URL, t->token);
            strncpy(fH.nextPageToken, t->token, sizeof(fH.nextPageToken) - 1);
            fH.nextPageToken[sizeof(fH.nextPageToken) - 1] = '\0';
        } else {
            snprintf(url, sizeof(url), "%s", IMDB_QUERY_URL);
            fH.nextPageToken[0] = '\0';
            free_titles_response(t);
            break;
        }
        fseek(binFp, 0, SEEK_SET);
        fwrite(&fH, sizeof(FileHeader), 1, binFp);
        fseek(binFp, 0, SEEK_END);
        free_titles_response(t);
    }while (fH.recordCount < 2378285 && fH.nextPageToken[0] != '\0');
    fclose(binFp);
}

int is_file_empty(FILE *fp) {
    fseek(fp, 0, SEEK_END);      // go to end
    long size = ftell(fp);       // get position (file size)
    rewind(fp);                  // return to start
    return size == 0;
}
/*
 *streamlined logic for parsing awards from requests
 */
void parse_awards() {

}