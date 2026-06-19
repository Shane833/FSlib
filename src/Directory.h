#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <path.h>

typedef struct{
    Path *dir;
    DIR *dirptr;
}Directory;

Directory *Directory_open(const char *dirpath);
int Directory_getDirEnt(Directory *dir, DArray *entries);
void Directory_close(Directory *dir);
#endif
