//
// Created by joaop on 18/11/2025.
//
//utilities library for the project
#ifndef IMDB_QUERY_UTIL_H
#define IMDB_QUERY_UTIL_H
#include <stdio.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};


size_t WriteMemoryCallback(const void *contents, size_t size, size_t nmemb, void *userp);

/**
 *
 * @param fp pointer to file
 * @return pointer to file buffer
 */
char *read_entire_file(FILE *fp);

/**
 *
 * @param fp file pointer
 * @return returns false if file is empty
 */
int is_file_empty(FILE *fp);


#endif //IMDB_QUERY_UTIL_H