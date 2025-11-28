//
// Created by joaop on 28/11/2025.
//

#ifndef IMDB_QUERY_APIHANDLER_H
#define IMDB_QUERY_APIHANDLER_H
#include <stdio.h>

#define IMDB_QUERY_URL "https://api.imdbapi.dev/titles"

/**
 * @param url
 * @param fp file pointer
 * @return false - if ok \n
 * @return true - if error has occurred \n\n
 * gets info from api referenced by the @code url@endcode and inserts it into the desired @code fileName@endcode
 *
 *- appends every request to the end of the file
*/
int get_info(char* url, FILE* fp);


#endif //IMDB_QUERY_APIHANDLER_H