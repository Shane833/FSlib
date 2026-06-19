#include <file.h>
#include "minunit.h"
#undef NDEBUG

File *file = NULL;

char *test_create(){
    file = File_open("tests/file.txt", READWRITE_ONLY);
    mu_assert(file != NULL, "Failed to open file!");

    return NULL;
}

char *test_readline(){
    /*
    while(File_readline(file) == 0){
    
        printf("%d: %s",file->current_line->line_no, bdata(file->current_line->data));
    
    }
    */
    mu_assert(File_readline(file) == 0, "Failed to read line!");
    fprintf(stderr, "%llu: %s",file->current_line->line_no, bdata(file->current_line->data));

    return NULL;
}

char *test_reset(){
    File_reset(file);

    mu_assert(ftell(file->fileptr) == 0, "Failed to reset the file position!");
    mu_assert(file->current_line->line_no == 0, "Failed to reset the current line!");

    return NULL;
}

char *test_readlines(){
    mu_assert(File_readlines(file) == 0, "Failed to read lines!");

    for(size_t i = 0;i < DArray_count(file->lines);i++){
        Line *line = (Line*)DArray_get(file->lines, i);
        fprintf(stderr, "%llu:%s",line->line_no, bdata(line->data));
    }
   
    return NULL;
}


char *test_search(){

    // Assuming to have less than 50 instances of the word to be found initially
    DArray *result = DArray_create(sizeof(bstring), 50); 
    mu_assert(result != NULL, "Failed to create DArray result!");
    
    // word to be searched for
    bstring word = bfromcstr("line");
    mu_assert(word != NULL, "Failed to create word for search!");

    mu_assert(File_search(file, word, result) == 0, "File Search failed!");
    
    // Print the instances if anything is found
    for(size_t i = 0;i < DArray_count(result);i++){
        bstring line = (bstring)DArray_get(result, i);
        if(line){
            fprintf(stderr, "%s", bdata(line));
        }
    }

    bdestroy(word);
    word = NULL;

    for(size_t i = 0;i < DArray_count(result);i++){
        bstring line = (bstring)DArray_get(result, i);
        if(line){
            bdestroy(line);
            line= NULL;
        }
    }

    DArray_destroy(result);
    result = NULL;
    
    return NULL;
}

char *test_close(){
    File_close(file);

    return NULL;
}

char *all_tests(){
    mu_suite_start(); 

    mu_run_test(test_create);
    mu_run_test(test_readline);
    //mu_run_test(test_reset);
    //mu_run_test(test_readlines);
    //mu_run_test(test_search);
    //mu_run_test(test_readline);
    mu_run_test(test_close);

    return NULL;
}

RUN_TESTS(all_tests);
