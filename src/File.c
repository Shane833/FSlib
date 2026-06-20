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
        debug("End of File reached!");
        return EOF;
    }

    return 0;
error:
    return -1;
}

int File_readlines(File *file, DArray *lines){
    check(file != NULL, "Invalid File Object!");
    check(lines != NULL, "Invalid lines DArray provided!");

    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");

    // Reset the file position 
    check(fseek(file->fileptr, 0, SEEK_SET) == 0, "Failed to set the file position!");

    bstring line = bfromcstr("");
    check(line != NULL, "Failed to create line bstring!");

    char c = '\0';

    while(!feof(file->fileptr)){
        c = fgetc(file->fileptr);

        if(c == '\n'){
            bconchar(line, c); 
            check(DArray_push(lines, line) == 0, "Failed to push line!");
            
            c = fgetc(file->fileptr);

            if(!feof(file->fileptr)){
                line = bfromcstr("");
                check(line != NULL, "Failed to create line bstring!");        

                bconchar(line, c);
            }
        }else{
            bconchar(line, c);     
        }
    }
    
    // Go back to the same file position
    check(fseek(file->fileptr, pos, SEEK_SET) == 0, "Failed to set the file position!");
    
    return 0;
error:
    return -1;
}

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
    
    fprintf(file->fileptr, "%s\n", bdata(line));
    
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

int File_tail(File *file, size_t no_lines, DArray *lines){
    check(file != NULL, "Invalid File Object!");
    check(lines != NULL, "Invalid lines DArray provided!");

    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");

    fseek(file->fileptr, -1L, SEEK_END); // Moving to the end of the file
    size_t no_of_new_lines = 0; // Counting the no. of lines processed 
    bool last_line_found = false; 
    
    bstring line = NULL;
    Stack *char_stk = Stack_create();
    Stack *line_stk = Stack_create();

    while(true){
        if(ftell(file->fileptr) == 0){
            char c = fgetc(file->fileptr);
            Stack_push(char_stk, (void *)c); 
            line = bfromcstr("");

            while(Stack_count(char_stk) > 0){
                char ch = (char)Stack_pop(char_stk);
                bconchar(line, ch);
            }

            Stack_push(line_stk, line);

            debug("Start of file reached!");
            break;
        }

        char c = fgetc(file->fileptr); // Read the character from the file

        // 1. First Identify the point where first character of the last line starts
        if(!last_line_found){
           if(c != '\n'){
               last_line_found = true;
               log_info("Last Line Found!");
               Stack_push(char_stk, (void *)'\n');
           }
        }
        else{
            if(c == '\n'){ // The next time we hit a newline it would be the end of
                           // the current line
                line = bfromcstr("");

                while(Stack_count(char_stk) > 0){
                    char ch = (char)Stack_pop(char_stk);
                    bconchar(line, ch);
                }
                
                Stack_push(line_stk, line); 
                
                Stack_push(char_stk, (void *)'\n');

                no_of_new_lines++;
                if(no_of_new_lines == no_lines){
                    break;
                }
            }else{
                Stack_push(char_stk, (void *)c);
            }
        }
        
        fseek(file->fileptr, -2L, SEEK_CUR); // Move back by 2, but fgetc moves forward by 1
                                             // Hence overall effect is equivalent to moving
                                             // back by 1
    }
    
    // Copy the lines into the lines DArray
    while(Stack_count(line_stk) > 0){
        bstring line = (bstring)Stack_pop(line_stk);
        DArray_push(lines, line);
    }

    Stack_destroy(char_stk);
    Stack_destroy(line_stk);

    // Go back to the same file position
    check(fseek(file->fileptr, pos, SEEK_SET) == 0, "Failed to set the file position!");

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
