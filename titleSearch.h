//
// Created by andri on 06/12/2025.
//

#ifndef TITLESEARCH_H
#define TITLESEARCH_H

#include <stdint.h>
#include <stdio.h>

#define DICT_SIZE 50021   // very large prime number for hash table size
#define BLOCK_SIZE 128    // postings block size for scalable index

/**
 * In-memory posting list structure
 */
typedef struct {
    char *term; // token
    int *ids; // vector of IDs (RAM only)
    int count; // used entries
    int capacity; // allocated entries
} PostingList;

/**
 * Structure for a block in the postings list file.
 * Each block contains a fixed number of document IDs and a pointer to the next block.
 */
typedef struct {
    int count; // how many IDs this block contains
    int64_t nextOffset; // next block offset (0 = end of chain)
    int ids[BLOCK_SIZE]; // fixed-size payload
} PostingBlock;

/**
 * Vocabulary entry structure stored in vocabulary.bin
 */
typedef struct {
    char term[32]; // normalized token
    int64_t firstBlockOffset; // offset of first block in postings.bin
} VocabularyEntry;

/**
 * Temporary dictionary entry for in-memory indexing
 */
typedef struct {
    char* term;
    int count;
    int* ids;
} DictEntry;

/** In-memory dictionary for indexing
 */
extern PostingList dictionary[DICT_SIZE];

/**
 * Removes accents from a character
 * @param c input character
 * @return character without accent
 */
char remove_accent(unsigned char c);

/**
 * Normalizes a title by lowercasing, removing accents, and stripping punctuation
 * @param input raw title
 * @param output normalized title
 */
void normalize_title(char *input, char *output);

/**
 * Tokenizes a normalized title and indexes each token
 * @param normalized normalized title
 * @param id title ID
 */
void tokenize_and_index(char *normalized, int id);

/**
 * Hashes a token to an index in the dictionary
 * @param s input token
 * @return hash index
 */
unsigned int hash_token(char *s);

/**
 * Inserts a token and its associated title ID into the in-memory dictionary
 * @param token input token
 * @param id title ID
 */
void insert_token_into_dictionary(char *token, int id);

/**
 * Checks if a token is a stopword
 * @param token input token
 * @return 1 if stopword, 0 otherwise
 */
int is_stopword(char *token);

/**
 * Writes a scalable postings list to postings.bin
 * @param posts file pointer to postings.bin
 * @param ids array of title IDs
 * @param count number of IDs
 * @return offset of the first block in the postings list
 */
int64_t write_posting_list(FILE* posts, int* ids, int count);

/**
 * Saves vocabulary + postings to disk
 * @param vocabFile path to vocabulary.bin
 * @param postingsFile path to postings.bin
 */
void save_dictionary(const char* vocabFile, const char* postingsFile);

/**
 * Compares two dictionary entries by term
 */
int compare_terms(const void* a, const void* b);
/**
 * Compares two integers
 */
int compare_int(const void* a, const void* b);

/**
 * Finds a term in the vocabulary.bin
 * @param term input token
 * @return VocabularyEntry struct (firstBlockOffset = -1 if not found)
 */
VocabularyEntry find_in_vocabulary(char* term);

/**
 * Loads postings list from postings.bin given an offset
 * @param offset offset of the first block in the postings list
 * @param outCount output parameter for number of IDs loaded
 * @return array of title IDs
 */
int* load_postings(int64_t offset, int* outCount);

/**
 * Searches for a term and retrieves associated title IDs
 * @param term input token
 * @param outCount output parameter for number of IDs found
 * @return array of title IDs
 */
int* search_term(char* term, int* outCount);

#endif //TITLESEARCH_H
