#include <file.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Stack.h>

File *File_Open(const char *filepath, ACCESS_MODE mode){
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
    if(temp) File_Close(temp);
    return NULL;
}

// This function performs better than the above implementation
int File_Readline(File *file, bstring line){
    check(file != NULL, "Invalid File Object!");
    check(file->fileptr != NULL, "No file opened!");

    // Check for the correct file permissions
    check(file->fmode == READ_ONLY || file->fmode == READWRITE_ONLY, "Invalid File Permission for Read Operation!");

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
        //return EOF;
        return -1;
    }

    return 0;
error:
    return -1;
}


int File_Readlines(File *file, DArray *lines){
    check(file != NULL, "Invalid File Object!");
    // Check for the correct file permissions
    check(file->fmode == READ_ONLY || file->fmode == READWRITE_ONLY, "Invalid File Permission for Read Operation!");
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


int File_Reset(File *file){
    check(file != NULL, "Invalid File Object!");
    // Reset file position OR I could've called rewind(file->fileptr)
    check(fseek(file->fileptr, 0, SEEK_SET) == 0, "Failed to set the file position!");

    return 0;
error:
    return -1;
}


int File_Writeline(File *file, bstring line){
    check(file != NULL, "Invalid File Object!");    

    // Check the access mode
    check(file->fmode == WRITE_ONLY || file->fmode == READWRITE_ONLY\
          || file->fmode == APPEND_ONLY, "Invalid File Permission for Write Operation!");
    
    // Check if the line have some data
    check(line != NULL, "Invalid line!");
    
    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");
    
    check(fseek(file->fileptr, 0, SEEK_END) == 0, "Failed to move to the end of the file!");
    fprintf(file->fileptr, "%s\n", bdata(line));

    // Go back to the same file position
    check(fseek(file->fileptr, pos, SEEK_SET) == 0, "Failed to set the file position!");

    return 0;
error:
    return -1;
}


int File_Writelines(File *file, DArray *lines){
    check(file != NULL, "Invalid File Object!"); 

    // Check the access mode
    check(file->fmode == WRITE_ONLY || file->fmode == READWRITE_ONLY\
          || file->fmode == APPEND_ONLY, "Invalid File Permission for Write Operation!");

    // Check the DArray lines if it contains any data
    check(lines != NULL, "Invalid DArray lines!");

    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");
    
    check(fseek(file->fileptr, 0, SEEK_END) == 0, "Failed to move to the end of the file!");
    
    for(int i = 0;i < DArray_count(lines);i++){
        bstring line = (bstring)DArray_get(lines, i);
        fprintf(file->fileptr, "%s\n", bdata(line));
    }

    // Go back to the same file position
    check(fseek(file->fileptr, pos, SEEK_SET) == 0, "Failed to set the file position!");

    return 0;
error:
    return -1;
}


int File_Tail(File *file, size_t no_lines, DArray *lines){
    check(file != NULL, "Invalid File Object!");
    // Check for the correct file permissions
    check(file->fmode == READ_ONLY || file->fmode == READWRITE_ONLY, "Invalid File Permission for Tail Operation!");
    check(lines != NULL, "Invalid lines DArray provided!");

    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");

    fseek(file->fileptr, 0L, SEEK_END); // Moving to the end of the file
    size_t no_of_new_lines = 0; // Counting the no. of lines processed 
    bool last_line_found = false; 
   
    // Variables to be used ahead
    bstring line = NULL;
    Stack *char_stk = Stack_create(); // stores the characters read from the stream
    check(char_stk != NULL, "Failed to create character stack!");
    Stack *line_stk = Stack_create(); // stores the lines formed from the characters
    check(line_stk != NULL, "Failed to create line stack!");

    //while(true){
      while(no_of_new_lines < no_lines){
          // If we are at the start of the file
        if(ftell(file->fileptr) == 0){
            char c = fgetc(file->fileptr); // fetch the last characeter
            Stack_push(char_stk, (void *)c); 

            line = bfromcstr("");
            check(line != NULL, "Failed to create line bstring!");

            while(Stack_count(char_stk) > 0){ // Create a line from the read characters
                char ch = (char)Stack_pop(char_stk);
                check(bconchar(line, ch) == BSTR_OK, "Failed to concatenate character onto line");
            }

            Stack_push(line_stk, line); // Adding the line to the stack

            debug("Start of file reached!");
            break;
        }

        char c = fgetc(file->fileptr); // Read the character from the file

        // 1. First Identify the point where first character of the last line starts
        if(!last_line_found){
           if(c != '\n'){
               last_line_found = true;
               debug("Last Line Found!");
               Stack_push(char_stk, (void *)'\n'); 
           }
        }
        else{
            if(c == '\n'){ // The next time we hit a newline it would be the end of
                           // the current line
                line = bfromcstr("");
                check(line != NULL, "Failed to create line bstring!");

                while(Stack_count(char_stk) > 0){// Create a line from the read characters
                    char ch = (char)Stack_pop(char_stk);
                    check(bconchar(line, ch) == BSTR_OK, "Failed to concatenate character onto line");
                }
                
                Stack_push(line_stk, line); // Adding the line to the stack
                
                Stack_push(char_stk, (void *)'\n');

                no_of_new_lines++; // increment the no. of lines read till now

            }else{
                Stack_push(char_stk, (void *)c); // Since on a 64-bit machine any pointer is 8 bytes
                                                 // more than enough to hold a single byte char
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
    if(char_stk)
        Stack_destroy(char_stk);
    if(line_stk)
        Stack_destroy(line_stk);

    return -1;
}


int File_Head(File *file, size_t no_lines, DArray *lines){
    check(file != NULL, "Invalid File Object!");
    check(file->fileptr != NULL, "No file opened!");

    // Check for the correct file permissions
    check(file->fmode == READ_ONLY || file->fmode == READWRITE_ONLY, "Invalid File Permission for Read Operation!");
    
    // store the current position in the file
    long pos = ftell(file->fileptr);
    check(pos != -1, "Failed to get the file position!");
    
    // Reset the file position 
    check(fseek(file->fileptr, 0, SEEK_SET) == 0, "Failed to set the file position!");

    // Use the already defined readline function
    for(int i = 0;i < no_lines;i++){
        bstring line = bfromcstr("");
        check(line != NULL, "Failed to create a line bstring!");

        int status = File_Readline(file, line);
        
        if(feof(file->fileptr) || status == -1){ 
            bdestroy(line); // deallocate this line  
            break;
        }

        DArray_push(lines, line);
   } 
   
    // Go back to the same file position
    check(fseek(file->fileptr, pos, SEEK_SET) == 0, "Failed to set the file position!");

   return 0;
error:
   return -1;
}


void File_Close(File *file){
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
