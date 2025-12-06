//
// Created by andri on 06/12/2025.
//

#include "titleSearch.h"
#include <ctype.h>
#include <string.h>

char remove_accent(unsigned char c) {
    // UTF-8 lower accents
    if (c == 0xA1 || c == 0xA0 || c == 0xA3 || c == 0xA2 || c == 0xA4) return 'a';
    if (c == 0xA9 || c == 0xA8 || c == 0xAA || c == 0xAB) return 'e';
    if (c == 0xAD || c == 0xAC || c == 0xAE || c == 0xAF) return 'i';
    if (c == 0xB3 || c == 0xB2 || c == 0xB5 || c == 0xB4 || c == 0xB6) return 'o';
    if (c == 0xBA || c == 0xB9 || c == 0xBB || c == 0xBC) return 'u';
    if (c == 0xA7) return 'c';

    // UTF-8 upper accents
    if (c == 0x81 || c == 0x80 || c == 0x83 || c == 0x82 || c == 0x84) return 'a';
    if (c == 0x89 || c == 0x88 || c == 0x8A || c == 0x8B) return 'e';
    if (c == 0x8D || c == 0x8C || c == 0x8E || c == 0x8F) return 'i';
    if (c == 0x93 || c == 0x92 || c == 0x95 || c == 0x94 || c == 0x96) return 'o';
    if (c == 0x9A || c == 0x99 || c == 0x9B || c == 0x9C) return 'u';
    if (c == 0x87) return 'c';

    return tolower(c);
}

void normalize_title(char* input, char* output) {
    int j = 0;
    int last_was_space = 1;

    for (int i = 0; input[i] != '\0'; i++) {
        unsigned char c = input[i];
        char clean = remove_accent(c);

        // Letras, números ou espaços
        if (isalnum(clean)) {
            output[j++] = clean;
            last_was_space = 0;
        }
        else if (isspace(clean)) {
            // evita múltiplos espaços
            if (!last_was_space) {
                output[j++] = ' ';
                last_was_space = 1;
            }
        }
        else {
            // ignora símbolos como !, ?, :, -, /, etc.
            continue;
        }
    }

    // remove espaço final
    if (j > 0 && output[j-1] == ' ')
        j--;

    output[j] = '\0';
}