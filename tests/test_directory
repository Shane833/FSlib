#include <directory.h>
#include "minunit.h"

Directory *dir = NULL;

char *test_dirEnt(){
    dir = Directory_open(".");
    mu_assert(dir != NULL, "Failed to open directory!");

    DArray *entries = DArray_create(sizeof(bstring), 20);
    mu_assert(entries != NULL, "Failed to create entries DArray!");

    mu_assert(Directory_getDirEnt(dir, entries) == 0, "Failed to get directory entries!");

    // printing the entries
    printf("Directory Entries : \n");
    for(int i = 0;i < DArray_count(entries);i++){
        printf("%s\n", bdata( (bstring) DArray_get(entries,i) ) );
    }

    for(int i = 0;i < DArray_count(entries);i++){
        bstring temp = (bstring)DArray_get(entries, i);
        if(temp){
            bdestroy(temp);
        }
    }
    DArray_destroy(entries);

    Directory_close(dir);

    return NULL;
}
char *all_tests(){
    mu_suite_start();
    mu_run_test(test_dirEnt);
    return NULL;
}

RUN_TESTS(all_tests);
