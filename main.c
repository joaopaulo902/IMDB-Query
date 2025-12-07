#include "binService.h"
#include "dbContext.h"
#include "sysManager.h"
#include "titleSearch.h"

int main(void) {
    make_titles_full_request();

    //initialize_system();


    int count;
    int* ids = search_term("stranger", &count);

    if (!ids) {
        printf("Nenhum título encontrado.\n");
    } else {
        printf("Encontrados %d títulos:\n", count);
        for (int i = 0; i < count; i++) {
            printf("ID: %d\n", ids[i]);
            Titles titles = get_title_by_id(ids[i]);
            printf("Título: %s\n", titles.primaryTitle);
            printf("Ano: %d\n", titles.startYear);
        }
        free(ids); // não esqueça!
    }

    return 0;
}
