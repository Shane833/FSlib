#include <file.h>
#include "minunit.h"
#include "profile.h"

#undef NDEBUG

File *file = NULL;

char *test_create(){
    //file = File_open("tests/file.txt", READWRITE_ONLY);
    file = File_open("tests/file.log", READWRITE_ONLY);
    mu_assert(file != NULL, "Failed to open file!");

    return NULL;
}

char *test_readline(){
    bstring line = bfromcstr("");
    mu_assert(line != NULL, "Failed to create our test line bstring!");
    //mu_assert(File_readline1(file, line) == 0, "Failed to read line!");
    mu_assert(File_readline(file, line) == 0, "Failed to read line!");
    fprintf(stderr, "%s", bdata(line));
    
    bdestroy(line);
    line = NULL;
    
    /*
    File_reset(file);

    line = bfromcstr("");
    mu_assert(line != NULL, "Failed to create our test line bstring!");
    mu_assert(File_readline2(file, line) == 0, "Failed to read line!");
    //fprintf(stderr, "%llu: %s",file->current_line->line_no, bdata(file->current_line->data));
    fprintf(stderr, "%s", bdata(line));

    bdestroy(line);
    line = NULL;
    */

    return NULL;
}

/*
char *readline1_cmp(){
    File_reset(file);
        
    bstring line = bfromcstr("");
    mu_assert(line != NULL, "Failed to create our test line bstring!");

    while(!feof(file->fileptr)){
        mu_assert(File_readline1(file, line) == 0, "Failed to read lines from the file!");
    }

    bdestroy(line);
    line = NULL;

    return NULL;
}

char *readline2_cmp(){
    File_reset(file);

    bstring line = NULL;

    while(!feof(file->fileptr)){
        line = bfromcstr("");
        mu_assert(File_readline2(file, line) == 0, "Failed to read lines from file!");
        bdestroy(line); 
    }
    
    return NULL;
}

char *test_readline_compare(){
   measure_runtime_ms(readline1_cmp); 
   measure_runtime_ms(readline2_cmp);

   return NULL;
}
*/

char *test_reset(){
    File_reset(file);

    mu_assert(ftell(file->fileptr) == 0, "Failed to reset the file position!");
    //mu_assert(file->current_line->line_no == 0, "Failed to reset the current line!");

    return NULL;
}

/*
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
*/

char *test_tail(){
    DArray *lines = DArray_create(sizeof(bstring), 5);
    mu_assert(lines != NULL, "Failed to create lines DArray!");

    File_tail(file, 5, lines);
    
    DArray_destroy(lines);
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
    mu_run_test(test_tail);
    //mu_run_test(test_readline_compare);
    //mu_run_test(test_reset);
    //mu_run_test(test_readlines);
    //mu_run_test(test_search);
    //mu_run_test(test_readline);
    mu_run_test(test_close);

    return NULL;
}

RUN_TESTS(all_tests);
