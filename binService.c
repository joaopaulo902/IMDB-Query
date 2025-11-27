//
// Created by joaop on 27/11/2025.
//

#include "binService.h"
#include <curl/curl.h>
#include <stdio.h>
#include "IQuery.h"
#include "entities.h"

void put_title(Titles entry, ParseTitle titlesArray, FileHeader f, FILE* fp) {
    //copy data into fixed size strings
    strncpy(entry.IMDBid, titlesArray.id, sizeof(entry.IMDBid) - 1);
    strncpy(entry.type, titlesArray.type, sizeof(entry.type) - 1);
    strncpy(entry.primaryTitle, titlesArray.primaryTitle, sizeof(entry.primaryTitle) - 1);
    if (titlesArray.plot) {
        strncpy(entry.plot, titlesArray.plot, sizeof(entry.plot) - 1);
        entry.plot[sizeof(entry.plot) - 1] = '\0';
    } else {
        entry.plot[0] = '\0';  // safe empty string
    }
    //truncate end of data by substituting it with '\0' in case of overflow
    entry.IMDBid[sizeof(entry.IMDBid) - 1] = '\0';
    entry.type[sizeof(entry.type) - 1] = '\0';
    entry.primaryTitle[sizeof(entry.primaryTitle) - 1] = '\0';

    //write data into file
    fwrite(&entry , sizeof(Titles), 1, fp);
}