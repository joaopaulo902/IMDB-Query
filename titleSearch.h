//
// Created by andri on 06/12/2025.
//

#ifndef TITLESEARCH_H
#define TITLESEARCH_H

#include <ctype.h>
#include <string.h>

char remove_accent(unsigned char c);

void normalize_title(char* input, char* output);

#endif //TITLESEARCH_H
