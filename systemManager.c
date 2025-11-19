//
// Created by andri on 19/11/2025.
//

#include "systemManager.h"
#include <stdio.h>

void initializeSystem() {
    FILE *ptFile;
    ptFile = fopen("title.txt", "r");
    if (ptFile != NULL) {
        while (!feof(ptFile)) {
            char letter = fgetc(ptFile);
            printf("%c", letter);
        }
        fclose(ptFile);
    } else {
        printf("Error opening log file.\n");
    }

    return 1;
}
