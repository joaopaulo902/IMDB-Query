#include "dbContext.h"
#include "entities.h"
#include "apiHandler.h"
#include "binService.h"
#include "filterGenre.h"
#include <string.h>
#include "bpTree.h"
#include "titleSearch.h"

#define MAX_DATA 500000

void populate_database() {
    char url[1024] = {IMDB_QUERY_URL};
    int i = 0;
    FileHeader fH = {0};
    int rvalue = get_file_header(&fH, TITLES_BIN);

    if (rvalue != 0) {
        perror("error in get_file_header");
        return;
    }

    printf("%llu\n", fH.recordCount);

    if (fH.recordCount >= MAX_DATA) {
            fprintf(stderr ,"maximum record count reached");
            return;
    }
    printf("coletando dados...\n");
    BPTree *T = bpt_open(YEAR_INDEX_FILE);
    BPTree *R = bpt_open(RATING_INDEX_FILE);
    do {
        printf("%d ", i++);
        if (i != 0 && i % 20 == 0) {
            printf("\n");
        }
        TitlesResponse *t = malloc(sizeof(TitlesResponse));

        if (get_info(url, DATA_JSON_PATH) != 0) {
            printf("erro em get_info\n");
            break;
        }
        int pageCount = get_page_title_item(t, DATA_JSON_PATH);
        if (pageCount == -1) {
            perror("Failed to open data.json on reading");
            free_titles_response(t);
            break;
        }

        for (int j = 0; j < pageCount; j++) {
            Title lastEntry = record_title_on_binary(t->titles[j], fH, j, TITLES_BIN);
            bpt_insert(T, lastEntry.startYear, lastEntry.id, lastEntry.id);
            bpt_insert(R, lastEntry.rating.aggregateRating, lastEntry.id, lastEntry.id);
            insert_genre_index(t->titles[j], lastEntry);
        }

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
        update_file_header(&fH, TITLES_BIN);

        free_titles_response(t);
    } while (fH.recordCount < MAX_DATA && fH.nextPageToken[0] != '\0');
    bpt_close(T);
    bpt_close(R);

    save_dictionary(VOCABULARY_BIN_PATH, POSTINGS_BIN_PATH);
    printf("dados coletados ___('u')/___<===\n");
}

void get_more_titles() {
    FileHeader fH = {0};

    // Carrega header atual do arquivo de títulos
    int rvalue = get_file_header(&fH, TITLES_BIN);
    if (rvalue != 0) {
        perror("error in get_file_header");
        return;
    }

    if (fH.recordCount >= MAX_DATA) {
        fprintf(stderr, "maximum record count reached\n");
        return;
    }

    if (fH.nextPageToken[0] == '\0') {
        printf("Não há nextPageToken salvo no header – nada para continuar.\n");
        return;
    }

    printf("Coletando mais títulos...\n");

    // Monta URL a partir do nextPageToken atual
    char url[1024];
    snprintf(url, sizeof(url), "%s?pageToken=%s", IMDB_QUERY_URL, fH.nextPageToken);

    // Abre índices já existentes (ano e rating)
    BPTree *T = bpt_open(YEAR_INDEX_FILE);
    BPTree *R = bpt_open(RATING_INDEX_FILE);
    if (!T || !R) {
        fprintf(stderr, "Falha ao abrir índices B+Tree.\n");
        if (T) bpt_close(T);
        if (R) bpt_close(R);
        return;
    }

    // Faz o request à API e lê o JSON retornado
    TitlesResponse *t = malloc(sizeof(TitlesResponse));
    if (!t) {
        perror("malloc TitlesResponse");
        bpt_close(T);
        bpt_close(R);
        return;
    }

    if (get_info(url, DATA_JSON_PATH) != 0) {
        printf("erro em get_info\n");
        free(t);
        bpt_close(T);
        bpt_close(R);
        return;
    }

    int pageCount = get_page_title_item(t, DATA_JSON_PATH);
    if (pageCount <= 0) {
        perror("Failed to parse data.json");
        free_titles_response(t);
        bpt_close(T);
        bpt_close(R);
        return;
    }

    printf("Novos títulos recebidos da API: %d\n", pageCount);

    // Grava os novos títulos em titles.bin + indexa nas B+Trees e por gênero
    for (int j = 0; j < pageCount; j++) {
        Title lastEntry = record_title_on_binary(t->titles[j], fH, j, TITLES_BIN);

        // Indexar ano e rating (na árvore o "value_offset" está sendo o próprio id)
        bpt_insert(T, lastEntry.startYear,               lastEntry.id, lastEntry.id);
        bpt_insert(R, lastEntry.rating.aggregateRating,  lastEntry.id, lastEntry.id);

        // Indexar por gênero
        insert_genre_index(t->titles[j], lastEntry);
    }

    // Atualiza contagem total de registros no header
    fH.recordCount += pageCount;

    // Atualiza nextPageToken no header
    if (t->token != NULL && strlen(t->token) > 0) {
        snprintf(fH.nextPageToken, sizeof(fH.nextPageToken), "%s", t->token);
    } else {
        // Acabaram as páginas
        fH.nextPageToken[0] = '\0';
    }

    // Escreve header atualizado em titles.bin
    update_file_header(&fH, TITLES_BIN);

    // Fecha as árvores
    bpt_close(T);
    bpt_close(R);

    // Liberar response da API
    free_titles_response(t);

    printf("[DICT] Limpando dicionário em memória...\n");

    // Limpar dicionário em RAM
    for (int i = 0; i < DICT_SIZE; i++) {
        if (dictionary[i].term != NULL) {
            free(dictionary[i].term);
            dictionary[i].term = NULL;
        }
        if (dictionary[i].ids != NULL) {
            free(dictionary[i].ids);
            dictionary[i].ids = NULL;
        }
        dictionary[i].count = 0;
        dictionary[i].capacity = 0;
    }

    printf("[DICT] Reconstruindo dicionário a partir de titles.bin...\n");

    FILE *fp = fopen(TITLES_BIN, "rb");
    if (!fp) {
        perror("Erro abrindo titles.bin para rebuild de índice");
        return;
    }

    FileHeader fhAll;
    if (fread(&fhAll, sizeof(FileHeader), 1, fp) != 1) {
        perror("Erro lendo header de titles.bin");
        fclose(fp);
        return;
    }

    // Lê sequencialmente todos os títulos e re-indexa em memória
    for (uint64_t i = 0; i < fhAll.recordCount; i++) {
        Title entry;
        if (fread(&entry, sizeof(Title), 1, fp) != 1) {
            perror("Erro lendo título durante rebuild do índice");
            break;
        }

        add_title_name(entry.primaryTitle, entry.id);
    }

    fclose(fp);

    printf("[DICT] Salvando vocabulary.bin e postings.bin a partir do dicionário completo...\n");
    save_dictionary(VOCABULARY_BIN_PATH, POSTINGS_BIN_PATH);

    printf("Índices e arquivos de texto reconstruídos.\n");
}


int get_titles_count() {
    FileHeader fH = {0};
    int rvalue = get_file_header(&fH, TITLES_BIN);
    if (rvalue != 0) {
        perror("error in get_file_header");
        return -1;
    }
    return (int) fH.recordCount;
}
