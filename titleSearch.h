//
// Created by andri on 06/12/2025.
//

#ifndef TITLESEARCH_H
#define TITLESEARCH_H

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#define DICT_SIZE 50021   // very large prime number for hash table size

typedef struct {
    char* term;     // token
    int* ids;       // ID list
    int count;      // used size
    int capacity;   // alocated space
} PostingList;

typedef struct {
    char term[32];     // Normalized string
    int64_t offset;    // postings.bin offset
    int count;         // ID's count
} VocabularyEntry;

extern PostingList dictionary[DICT_SIZE];

char remove_accent(unsigned char c);

void normalize_title(char* input, char* output);
void tokenize_and_index(char* normalized, int id);
unsigned int hash_token(char* s);
void insert_token_into_dictionary(char* token, int id);
int is_stopword(char* token);

#endif //TITLESEARCH_H
