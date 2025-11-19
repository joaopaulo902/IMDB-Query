#include <stdio.h>
#include "systemManager.h"

int main(void) {

    int isRunning = 1;
    initializeSystem();

    do {
        isRunning = 0;
    } while (isRunning);

    return 0;
}
