#ifndef FILE_H
#define FILE_H

#include <path.h>

typedef enum {
    READ_ONLY,
    WRITE_ONLY,
    READWRITE_ONLY,
    APPEND_ONLY
}ACCESS_MODE;

// Represents a single line in the file
typedef struct{
    bstring data;
    size_t line_no;
}Line;

typedef struct{
    Path *file; 
    FILE *fileptr;
    /*
    Line *current_line;
    DArray *lines;
    */
    ACCESS_MODE fmode;
}File;

File *File_open(const char *filepath, ACCESS_MODE mode);

//int File_readline1(File *file, /*Output*/bstring line);
int File_readline(File *file, /*Output*/bstring line);
int File_readlines(File *file, /*Output*/DArray *lines);
int File_writeline(File *file, /*Input*/bstring line); // Provide a single line of data to write
int File_writelines(File *file, /*Input*/DArray *lines); // 
int File_tail(File *file, size_t no_lines, /*Output*/DArray *lines);
int File_head(File *file, size_t no_lines, /*Output*/DArray *lines);
// Searches a words in a file
int File_search(File *file, /*Input*/bstring word, /*Output*/DArray *result);

void reverse_string(unsigned char *str, size_t n); // Handy function to reverse a string

int File_reset(File *file);
void File_close(File *file);

#endif
