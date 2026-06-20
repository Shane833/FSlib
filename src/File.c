#include <file.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Stack.h>

File *File_open(const char *filepath, ACCESS_MODE mode){
    File *temp = NULL; //

    check(filepath != NULL, "Invalid file path!");
    check((Path_exists(filepath) && Path_isFile(filepath)) == true, "Invalid file path!");

    temp = malloc(sizeof(File));
    check(temp != NULL, "Failed to create file!");
    
    temp->file = Path_open(filepath);
    check(temp->file != NULL, "Failed to open file!");
    
    // Opening in binary to mode for better compatibility between
    // windows and linux such that \r\n and \n are equivalent
    temp->fmode = mode;

    switch(temp->fmode){
        case READ_ONLY:
            temp->fileptr = fopen(filepath, "rb");
            break;
        case WRITE_ONLY:
            temp->fileptr = fopen(filepath, "wb");
            break;
        case READWRITE_ONLY:
            temp->fileptr = fopen(filepath, "r+b");
            break;
        case APPEND_ONLY:
            temp->fileptr = fopen(filepath, "ab");
            break;
    }
    check(temp->fileptr != NULL, "Failed to open file!");
    
    return temp;
error:
    if(temp) File_close(temp);
    return NULL;
}

/* PERFORMS WORSE THEN OTHER FUNCTION 
int File_readline1(File *file, bstring line){
    check(file != NULL, "Invalid File Object!");
    check(file->fileptr != NULL, "No file opened!");

    size_t line_len = 0; 
    unsigned char c = '\0';

    while(!feof(file->fileptr)){
        c = fgetc(file->fileptr);
        line_len++;
        
        if(c == '\n'){
            fseek(file->fileptr, -(line_len), SEEK_CUR);
            // Get the line
            // First deallocate any existing data
            free(line->data);
            // Allocate the line data and copy the line
            line->data = calloc(1, line_len + 1);
            check(line->data != NULL, " Failed to initialize line data");
            line->mlen = line->slen = line_len; 
            // fgets, if successful returns the address of the passed buffer
            check(fgets(line->data, line_len + 1,\
                        file->fileptr) == line->data, "Failed to read the line!");

            break;
        }
        
    }
    
    // TODO: Fix the case when the file contains only one line
    // If we are already at the end of the file don't do anything 
    if(feof(file->fileptr)){
        fprintf(stderr, "[INFO] End of File reached!\n");
        //if(file->current_line->line_no == 0){ file->current_line->line_no = 1; }
        return 0; // but this is not a failed state, its completely fine 
    }else{
        return 0;
    }

error:
    // Reset the line
    if(line->data){
        free(line->data);
        line->data = NULL;
    }
    //file->current_line->line_no = 0;
    return -1;
}
*/

// This function performs better than the above implementation
int File_readline(File *file, bstring line){
    check(file != NULL, "Invalid File Object!");
    check(file->fileptr != NULL, "No file opened!");
    check(line != NULL, "Invalid line bstring provided!");

    unsigned char c = '\0';
    
    while(!feof(file->fileptr)){
        c = fgetc(file->fileptr);

        check(bconchar(line, c) == BSTR_OK, "Failed to copy char into the bstring!");
        if(c == '\n') break;
    }
    
    // TODO: Fix the case when the file contains only one line
    // If we are already at the end of the file don't do anything 
    if(feof(file->fileptr)){
        debug("[INFO] End of File reached!");
        return EOF;
    }

    return 0;
error:
    return -1;
}

/*
int File_readlines(File *file){
    check(file != NULL, "Invalid File Object!");
    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");

    // Reset the file position 
    check(fseek(file->fileptr, 0, SEEK_SET) == 0, "Failed to set the file position!");
    
    // Create our DArray of lines
    // Assuming initially the file to have 100 lines
    file->lines = DArray_create(sizeof(Line), 100);
    check(file->lines != NULL, "Failed to read lines!");

    bstring data = NULL;
    unsigned char c = '\0';
    size_t line_no = 0;
    size_t line_len = 0;

    while(!feof(file->fileptr)){
        c = fgetc(file->fileptr);
        line_len++;
        
        if(c == '\n'){
            Line *line = malloc(sizeof(Line));
            check(line != NULL, "Failed to initialize the line!");  
            
            line->data = malloc(sizeof(struct tagbstring));
            check(line->data != NULL, "Failed to initialize bstring!");
            
            line->data->data = calloc(1, line_len + 1);
            check(line->data->data != NULL, "Failed to initialize bstring data!");
            line->data->slen = line->data->mlen = line_len;
            // Moving back the file pointer
            //fseek(file->fileptr, -(line_len + 1), SEEK_CUR);
            fseek(file->fileptr, -(line_len), SEEK_CUR);
            // fgets, if successful returns the address of the passed buffer
            check(fgets(line->data->data, line_len + 1, file->fileptr) == line->data->data, "Failed to read the line!");

            line->line_no = ++line_no;
            check(DArray_push(file->lines, line) == 0, "Failed to push line!");
            
            line_len = 0;
        }
    }
    
    // Go back to the same file position
    check(fseek(file->fileptr, pos, SEEK_SET) == 0, "Failed to set the file position!");
    
    return 0;
error:
    if(file->lines){ 

        DArray_destroy(file->lines);
    }
    return -1;
}
*/

/*
// Reads the lines from the file one by one
// Searches for the provided word in those files
// If found adds the line no and the charcter position
// of the word found in the results DArray
// returns 0 on success and -1 if it fails
int File_search(File *file, bstring word, DArray *result){
    check(file != NULL, "Invalid File Object!");
    check(file->fileptr != NULL, "No file opened!");
    check(word != NULL, "Invalid word!")
    check(word->slen >= 0 && word->mlen >= word->slen && word->data != NULL, "Invalid word data!");
    check(result != NULL, "Invalid result DArray!");
        
    // Read all the lines
    check(File_readlines(file) == 0, "Failed to read lines!");
    // Go through each of the lines and search for the word
    for(size_t i = 0;i < DArray_count(file->lines); i++){
        Line *line = (Line *)DArray_get(file->lines, i);
        if(line){
            // Search for the word
            if(binstr(line->data, 0, word) != BSTR_ERR){
                // I'll be copying the whole line hence no need to
                // check the line again, if the word is found once
                // then we won't be checking for multiple occurences
                bstring found = NULL;
                
                found = bformat("(%s:%llu) : %s", bdata(file->file->path), line->line_no, bdata(line->data));
                check(found != NULL, "Failed to copy line data!");

                check(DArray_push(result, found) == 0,"Failed to add line!");
            }
        }
    }

   return 0;
error:
    return -1;
}
*/

int File_reset(File *file){
    check(file != NULL, "Invalid File Object!");
    // Reset file position OR I could've called rewind(file->fileptr)
    check(fseek(file->fileptr, 0, SEEK_SET) == 0, "Failed to set the file position!");

    return 0;
error:
    return -1;
}

int File_writeline(File *file, bstring line){
    check(file != NULL, "Invalid File Object!");    

    // Check the access mode
    check(file->fmode == WRITE_ONLY || file->fmode == READWRITE_ONLY\
          || file->fmode == APPEND_ONLY, "Invalid Access Mode, Failed to write line!");
    
    // Check if the line have some data
    check(line != NULL, "Invalid line!");
    
    check(blength(line) == fprintf(file->fileptr, "%s\n", bdata(line)), "Failed to write line to file!");
    
    return 0;
error:
    return -1;
}

int File_writelines(File *file, DArray *lines){
    check(file != NULL, "Invalid File Object!"); 

    // Check the access mode
    check(file->fmode == WRITE_ONLY || file->fmode == READWRITE_ONLY\
          || file->fmode == APPEND_ONLY, "Invalid Access Mode, Failed to write line!");

    // Check the DArray lines if it contains any data
    check(lines != NULL, "Invalid DArray lines!");

    return 0;
error:
    return -1;
}

// Utitlity function to reverse a string
/*static inline*/ void reverse_string(unsigned char *str, size_t n){
    for(int i = 0;i <= n / 2;i++){
       char temp = str[i];
       str[i] = str[n - i - 1];
       str[n - i - 1] = temp;
    } 
}

int File_tail(File *file, size_t no_lines, DArray *lines){
    check(file != NULL, "Invalid File Object!");
    check(lines != NULL, "Invalid lines DArray provided!");
    
    fseek(file->fileptr, -1L, SEEK_END); // Moving to the end of the file
    size_t no_of_new_lines = 0; // Counting the no. of lines processed 
    bool last_line_found = false; 
    
    bstring line = NULL;
    Stack *stk = Stack_create();

    while(true){
        if(ftell(file->fileptr) == 0){
            char c = fgetc(file->fileptr);
            //printf("FilePostion : %lld Char : %c\n", ftell(file->fileptr), c);
            bconchar(line, c);
            //reverse_string(bdata(line), blength(line));
            bconchar(line, '\n');
            //log_info("Line : %s", bdata(line));
            printf("Line : %s", bdata(line));

            debug("Start of file reached!");
            break;
        }

        char c = fgetc(file->fileptr); // Read the character from the file

        // 1. First Identify the point where first character of the last line starts
        if(!last_line_found){
           if(c != '\n'){
               last_line_found = true;
               log_info("Last Line Found!");
               //printf("%c\n", c);
               line = bfromcstr("\n"); // Allocating the bstring for the first time
            }
        }
        else{
            //printf("FilePostion : %lld Char : %c\n", ftell(file->fileptr), c);
            
            if(c == '\n'){ // The next time we hit a newline it would be the end of
                           // the current line
                //reverse_string(bdata(line), strlen(line->data));
                //bconchar(line, '\n');
                //log_info("Line : %s", bdata(line));
                printf("Line : %s", bdata(line));
                bdestroy(line);

                line = bfromcstr("\n");
                //printf("%c\n", c);
                no_of_new_lines++;
                //printf("New Line found!\n");
                if(no_of_new_lines == no_lines){
                    break;
                }
            }else{
                bconchar(line, c); // we will keep on appending the character into the string
                //printf("%c\n", c);
            }
        }
        
        fseek(file->fileptr, -2L, SEEK_CUR); // Move back by 2, but fgetc moves forward by 1
                                             // Hence overall effect is equivalent to moving
                                             // back by 1
    }

    return 0;
error:
    return -1;
}

void File_close(File *file){
    if(file){
        if(file->fileptr){
            fclose(file->fileptr);
            file->fileptr = NULL;
        }
        if(file->file){
            Path_close(file->file);
            file->file = NULL;
        }
        free(file);
    }
}
