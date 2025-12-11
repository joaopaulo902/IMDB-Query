//
// Created by joaop on 28/11/2025.
//
// "links api and json"
#ifndef IMDB_QUERY_DBCONTEXT_H
#define IMDB_QUERY_DBCONTEXT_H

/**
 * Makes repeated requests to the api until reaching max data or no more pages
 * stores data on binary files
 */
void populate_database();

/**
 * @return total number of titles stored in titles.bin
 */
int get_titles_count();

/**
 * Requests more titles from the API using the nextPageToken in the file header
 * and appends them to titles.bin
 */
void get_more_titles();
#endif //IMDB_QUERY_DBCONTEXT_H