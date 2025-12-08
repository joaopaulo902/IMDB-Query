//
// Created by joaop on 05/12/2025.
//

#ifndef IMDB_QUERY_POSTINGS_H
#define IMDB_QUERY_POSTINGS_H
#include "entities.h"

/**
 * Inserts the title into the corresponding genre index file
 * @param title the title's parsed data
 * @param entry the title's fixed size data
 */
void insert_genre_index(ParseTitle title, Title entry);

/**
 * Hash function for genre index
 * @param h input value
 * @return hashed value
 */
uint64_t hashFunction(uint64_t h);

/**
 * Test function for genre filter
 */
void test_filter(void);

#endif //IMDB_QUERY_POSTINGS_H