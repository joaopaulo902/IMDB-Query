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
void make_titles_full_request();

/**
 * @return total number of titles stored in titles.bin
 */
int get_titles_count();

#endif //IMDB_QUERY_DBCONTEXT_H