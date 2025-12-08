#ifndef IQUERY_H
#define IQUERY_H

#include "IQuery.h"
#include "entities.h"

/**
 * Query titles by page number
 * @param currentPage current page number
 * @param page array to store the titles of the current page
 * @param totalMovies total number of movies available
 */
void query_titles_by_page(int currentPage, Title* page, int totalMovies);

#endif //IQUERY_H

