#include <dbg.h>

static FILE *file = NULL;

void genericDebug(const char *current_file, int current_line, const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);

    #ifdef DEBUG_FILE
        if(!file){
            file = fopen("file.log", "ab"); // opens a file in the same directory
        }
                                       // opening it in append mode
        fprintf(file, "[DEBUG] (%s:%d) ", current_file, current_line);
        vfprintf(file, fmt, argp);
        fprintf(file, "\n");
        
        fflush(file);
    #else
        fprintf(stderr, "[DEBUG] (%s:%d) ", current_file, current_line);
        vfprintf(stderr, fmt, argp);
        fprintf(stderr, "\n");
    #endif
    va_end(argp);

}

void genericLog(const char *tag, const char *current_file, int current_line, const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    
    #ifdef DEBUG_FILE
        if(!file){
            file = fopen("file.log", "ab"); // opens a file in the same directory
        }
                                       // opening it in append mode
        fprintf(file, "%s (%s:%d: errno: %s) ",tag, current_file, current_line, clean_errno());
        vfprintf(file, fmt, argp);
        fprintf(file, "\n");
        
        fflush(file);
    #else
        fprintf(stderr, "%s (%s:%d: errno: %s) ",tag, current_file, current_line, clean_errno());
        vfprintf(stderr, fmt, argp);
        fprintf(stderr, "\n");
    #endif
    va_end(argp);
}

void closeLog(){
    if(file){ // closing the file
        fclose(file);
        file = NULL;
    }
}
