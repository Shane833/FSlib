#include <file.h>
#include "minunit.h"
#include "profile.h"

#undef NDEBUG

File *file = NULL;

char *test_create(){
    //file = File_open("tests/file.txt", READWRITE_ONLY);
    file = File_Open("tests/file.log", READWRITE_ONLY);
    mu_assert(file != NULL, "Failed to open file!");

    return NULL;
}


char *test_readline(){
    bstring line = bfromcstr("");
    mu_assert(line != NULL, "Failed to create our test line bstring!");
    mu_assert(File_Readline(file, line) != -1, "Failed to read line!");
    
    fprintf(stderr, "%s", bdata(line));
    
    bdestroy(line);
    line = NULL;

    return NULL;
}


char *test_reset(){
    File_Reset(file);

    mu_assert(ftell(file->fileptr) == 0, "Failed to reset the file position!");

    return NULL;
}


char *test_readlines(){
    DArray *lines = DArray_create(sizeof(bstring), 10);
    mu_assert(lines != NULL, "Failed to create lines DArray!");

    mu_assert(File_Readlines(file, lines) != -1, "Failed to read lines!");

    for(size_t i = 0;i < DArray_count(lines);i++){
        bstring line = (bstring)DArray_get(lines, i);
        printf("Line : %s", bdata(line));
        bdestroy(line);
    }

    DArray_destroy(lines);
   
    return NULL;
}


char *test_writeline(){
    bstring line = bfromcstr("This is a test line");
    mu_assert(line != NULL, "Failed to create the line bstring!");

    mu_assert(File_Writeline(file, line) == 0, "Failed to write the line in the file!");

    return NULL;
}


char *test_tail(){
    DArray *lines = DArray_create(sizeof(bstring), 10);
    mu_assert(lines != NULL, "Failed to create lines DArray!");

    mu_assert(File_Tail(file, 6, lines) != -1, "Failed to read the tail of file!");

    for(int i = 0;i < DArray_count(lines);i++){
        bstring line = (bstring)DArray_get(lines, i);
        printf("Line : %s", bdata(line));
        bdestroy(line);
    }

    DArray_destroy(lines);

    return NULL;
}


char *test_head(){
    DArray *lines = DArray_create(sizeof(bstring), 10);
    mu_assert(lines != NULL, "Failed to create lines DArray!");

    mu_assert(File_Head(file, 6, lines) != -1, "Failed to read the head of the file!");

    for(int i = 0;i < DArray_count(lines);i++){
        bstring line = (bstring)DArray_get(lines, i);
        printf("Line : %s", bdata(line));
        bdestroy(line);
    }

    DArray_destroy(lines);

    return NULL;
}


char *test_close(){
    File_Close(file);

    return NULL;
}


char *all_tests(){
    mu_suite_start(); 

    mu_run_test(test_create);
    mu_run_test(test_readline);
    mu_run_test(test_tail);
    mu_run_test(test_readlines);
    //mu_run_test(test_writeline);
    mu_run_test(test_head);
    mu_run_test(test_close);

    return NULL;
}

RUN_TESTS(all_tests) // removed ';' bcz ISO C doesn't allow ';' outside of functions
