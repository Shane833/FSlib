#include <path.h>
#include <stdlib.h>
#include <string.h>

// Building it for setting up files for now
static bstring getFileParent(const char *path){
    bstring parent = NULL;

    // We can search for the first '\\' character from the last
    // and then move until that pointer, if nothing is found
    // then this suggests that the file belongs to the same 
    // directory as the the binary itself
    // Checking for both type of slashes
   char *char_ptr = strrchr(path, '\\') == NULL ? (strrchr(path, '/') \
           == NULL ? NULL : strrchr(path,'/')) : strrchr(path, '\\');

   if(char_ptr){
      // then we would need to get the path before the file name
      //size_t parent_size = (char_ptr - path) + 1; // +1 to include '\\' character
      size_t parent_size = char_ptr - path;
      
      char *parent_ptr = calloc(1, parent_size + 1); // +1 for the null character
      check(parent_ptr != NULL, "Failed to initialize parent!");
      
      strncpy(parent_ptr, path, parent_size);
      
      parent = bfromcstr(parent_ptr);
      check(parent != NULL, "Failed to initialize parent");
        
      free(parent_ptr);
   }else{
       char *parent_ptr = calloc(1, sizeof(char) + 1);
       check(parent_ptr != NULL, "Failed to initialize parent!");
       
       strcpy(parent_ptr,".");
        
       parent = bfromcstr(parent_ptr);
       check(parent != NULL, "Failed to initialize parent");

       free(parent_ptr);
   } 

   return parent;
error:
   if(parent) bdestroy(parent);
   parent = NULL;
   return NULL;
}

static bstring getDirParent(const char *path){
    bstring parent = NULL;

   char *char_ptr = strrchr(path, '\\') == NULL ? (strrchr(path, '/') \
           == NULL ? NULL : strrchr(path,'/')) : strrchr(path, '\\');
    
   if(char_ptr){
       if(*(char_ptr + 1) == '\0'){ // This means its still the same directory
                                    // But this would still be true for other directories too
           // Hence we must check again
           /*
           char *parent_ptr = calloc(1, sizeof(char) + 1);
           check(parent_ptr != NULL, "Failed to initialize parent!");
        
           strcpy(parent_ptr,".");
             
           parent = bfromcstr(parent_ptr);
           check(parent != NULL, "Failed to initialize parent");

           free(parent_ptr);
           */
       }else{
        parent = bfromcstr(".");
        check(parent != NULL, "Failed to initialize parent!");
       }
  }else{
        parent = bfromcstr(".");
        check(parent != NULL, "Failed to initialize parent!");
  }

   return parent;
error:
   if(parent) bdestroy(parent);
   parent = NULL;
}

/*
inline int getParts(const char *path, DArray *parts){
    //char *path_cpy = strdup(path);
    //check(path_cpy != NULL, "Failed to duplicate string!");
}

inline bstring getFileStem(const char *path){

error:
    return NULL;
}

inline bstring getDirStem(const char *path){

error:
    return NULL;
}
*/

inline bstring Path_getFileSuffix(const char *filepath){
    check(filepath != NULL, "Invalid filepath!");
    
    bstring suffix = NULL;

   char *dot = strrchr(filepath, '.');
   if(dot){
        suffix = bfromcstr(dot);
        check(suffix != NULL, "Failed to get the suffix!");
   }else{
        suffix = bfromcstr(" ");
        check(suffix != NULL, "Failed to get the suffix!");
   }

   return suffix;
error:
    return NULL;
}

static inline void setupDirectory(Path *temp, const char *path){
    DIR *dir = opendir(path);
    check(dir != NULL, "Failed to open path!");

    temp->path = bfromcstr(dir->dd_name);
    check_debug(temp->path != NULL, "Failed to create path bstring!");
    
    closedir(dir);

    // Cleanup the asterisk - slen is the actual length of the string
    check_debug(bdelete(temp->path, temp->path->slen - 1, 1) == BSTR_OK,"Failed to modify the path string!");
    
    // Gettings its parent
    temp->parent = getDirParent(path);
    check(temp->parent != NULL, "Failed to create parent bstring!");

error:
    return;
}

static inline void setupFile(Path *temp, const char *path){
    temp->parent = getFileParent(path);
    check(temp->parent != NULL, "Failed to find parent");
    
    temp->path = bfromcstr(path);
    check(temp->path != NULL, "Failed to set path!");

error:
    return;
}

Path *Path_open(const char *path){
    check(path != NULL, "Invalid Path!");
    check(Path_exists(path) == true, "Something went wrong!");

    Path *temp = calloc(1, sizeof(Path));
    check(temp != NULL, "Failed to create Path object!");
    
    if(Path_isDir(path)){
       temp->is_dir = true;
       temp->is_file = false;
       setupDirectory(temp, path);
    }else if(Path_isFile(path)){
       temp->is_file = true;
       temp->is_dir = false;
       setupFile(temp, path);
    }

    return temp;
error:
    return NULL;
}

inline bool Path_isFile(const char *filepath){
    check(filepath != NULL, "Invalid file path!");
    
    FILE *file = fopen(filepath, "r");
    check_debug(file != NULL, "Unable to open file!");

    fclose(file);
    return true;
error:
    if(file) fclose(file);
    return false;
}

inline bool Path_isDir(const char *dirpath){
    check(dirpath != NULL, "Invalid directory path!");

    DIR *dir = opendir(dirpath);
    check_debug(dir != NULL, "Unable to open directory!");

    closedir(dir);
    return true;
error:
    if(dir) closedir(dir);
    return false;
}

inline bool Path_exists(const char *path){
    check(path != NULL, "Path does not exist!");
    return Path_isDir(path) || Path_isFile(path);
error:
    return false;
}

// Handy macro for identifying path type
#define PATHTYPE(path) Path_isDir((path)) ? "Directory" : (Path_isFile((path)) ? "File" : NULL)

inline char *Path_type(const char *path){
    check(path != NULL, "Invalid path!");
    return PATHTYPE(path);

error:
    return NULL;
}

// Destroying path object
void Path_close(Path *path){
    if(path){
       if(path->path) bdestroy(path->path);
       if(path->stem) bdestroy(path->stem);
       if(path->suffix) bdestroy(path->suffix);
       if(path->parent) bdestroy(path->parent);

       free(path); 
    }                                             
}
