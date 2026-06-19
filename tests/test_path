#include <path.h>
#include "minunit.h"
#include <string.h>
#include <dirent.h>

Path *file = NULL;
Path *dir = NULL;

char *test_open()
{
   /*
    file = Path_open("Makefile");
    mu_assert(file != NULL, "Failed to create path object");
    mu_assert(strcmp(file->parent, ".") == 0, "Wrong parent!");
    Path_destroy(file);

    file = Path_open("src/path.py");
    mu_assert(file != NULL, "Failed to create path object");
    mu_assert(strcmp(file->parent, "src") == 0, "Wrong parent!");
    Path_destroy(file);
    */

    //dir = Path_open("./src/");
    //dir = Path_open("./src");
    dir = Path_open("src/");
    //dir = Path_open("src\\");
    mu_assert(dir != NULL, "Failed to create dir path object");
    printf("File: %d, Directory: %d\n", dir->is_file, dir->is_dir);
    //mu_assert(strcmp(file->parent, "src") == 0, "Wrong parent!");
    
    //file = Path_open("src\\path.c");
    file = Path_open("src/path.c");
    //file = Path_open("Makefile");
    //file = Path_open("./Makefile");
    //file = Path_open(".\\Makefile");
    mu_assert(file != NULL, "Failed to create file path object");
    printf("File: %d, Directory: %d\n", file->is_file, file->is_dir);

    return NULL;
}
char *test_parent(){
    printf("Dir Parent : %s\n", bdata(dir->parent));
    printf("File Parent : %s\n", bdata(file->parent));

}
char *test_path(){
    printf("Dir Absolute Path : %s\n", bdata(dir->path));
    printf("File Absolute Path : %s\n", bdata(file->path));
}

char *test_suffix(){
    bstring ext = Path_getFileSuffix("src/path.c");
    mu_assert(ext != NULL, "Failed to find suffix!");
    bstring suf1 = bfromcstr(".c");
    mu_assert(bstrcmp(ext, suf1) == 0, "Wrong output of suffix!");

    ext = Path_getFileSuffix("Makefile");
    mu_assert(ext != NULL, "Failed to find suffix!");
    bstring suf2 = bfromcstr(" ");
    mu_assert(bstrcmp(ext, suf2) == 0, "Wrong output of suffix!");

    return NULL;
}

char *test_close(){
    Path_close(file);
    file = NULL;
    Path_close(dir);
    dir = NULL;

    return NULL;
}
char *all_tests(){
    mu_suite_start();
    /*
    mu_run_test(test_open);
    mu_run_test(test_parent);
    mu_run_test(test_path);
    mu_run_test(test_close);
    */
    mu_run_test(test_suffix);
    return NULL;
}

RUN_TESTS(all_tests);
