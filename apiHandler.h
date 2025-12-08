//
// Created by joaop on 28/11/2025.
//

#ifndef IMDB_QUERY_APIHANDLER_H
#define IMDB_QUERY_APIHANDLER_H

#define IMDB_QUERY_URL "https://api.imdbapi.dev/titles"
#define DATA_JSON_PATH "data.json"

/**
 * @param url
 * @param fileName name of the file in which the data will be store
 * @return false - if ok \n
 * @return true - if error has occurred \n\n
 * gets info from api referenced by the @code url@endcode and inserts it into the desired @code fileName@endcode
 * creates a new instance of the file every call
*/
int get_info(char* url, char fileName[]);


#endif //IMDB_QUERY_APIHANDLER_H