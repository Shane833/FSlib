#ifndef FILE_H
#define FILE_H

#include <path.h>

typedef enum {
    READ_ONLY,
    WRITE_ONLY,
    READWRITE_ONLY,
    APPEND_ONLY
}ACCESS_MODE;

typedef struct{
    Path *file; 
    FILE *fileptr;
    ACCESS_MODE fmode;
}File;

File *File_Open(const char *filepath, ACCESS_MODE mode);

int File_Readline(File *file, /*Output*/bstring line);

int File_Readlines(File *file, /*Output*/DArray *lines);

int File_Writeline(File *file, /*Input*/bstring line); // Provide a single line of data to write

int File_Writelines(File *file, /*Input*/DArray *lines); // 

int File_Tail(File *file, size_t no_lines, /*Output*/DArray *lines);

int File_Head(File *file, size_t no_lines, /*Output*/DArray *lines);

int File_Reset(File *file);

void File_Close(File *file);

#endif
