#include "dbContext.h"
#include "sysManager.h"
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char buffer[16];
    char cmd;
    do {
        printf("Enter your command:\n[P] Populate database \n[G] Get more 50 items \n[I] Enter System\n");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        cmd = buffer[0];
        switch (toupper(cmd)) {
            case 'p':
            case 'P':
                populate_database();
                printf("\n");
                break;
            case 'I':
            case 'i':
                initialize_system();
                return 0;
                break;
            case 'G':
            case 'g':
                get_more_titles();
                printf("\n");
            default:
                continue;
        }
    } while (toupper(cmd) != 'L');


    //test_filter();
    //BP_tree_test();

    return 0;
}
