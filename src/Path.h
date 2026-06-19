/* Trying to provide an API similar to pathlib in python */
#ifndef PATH_H
#define PATH_H

#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <bstrlib.h>
#include <DArray.h>
#include <dbg.h>

// Declaring the struct
typedef struct{
    bstring parent; // parent of the file/dir
    bstring stem; // name of the file or directory
    bstring suffix; // extension if its a file
    bstring path; // absolute path of the provided file/dir

    bool is_file;
    bool is_dir;
}Path;

Path *Path_open(const char *path);

bool Path_exists(const char *path);
bool Path_isFile(const char *path);
bool Path_isDir(const char *path);
bstring Path_getFileSuffix(const char *filepath);

// Useless function
char *Path_type(const char *path);

void Path_close(Path *path);

#endif
