#include <directory.h>

Directory* Directory_open(const char *dirpath){
    check(dirpath != NULL, "Invalid path!");
    check(Path_isDir(dirpath) == true, "Directory does not exist!");
    
    Directory *temp = malloc(sizeof(Directory));
    check(temp != NULL, "Failed to create Directory Object!");

    temp->dirptr = opendir(dirpath);
    check(temp->dirptr != NULL, "Failed to open directory!");

    temp->dir = Path_open(dirpath);
    check(temp->dir != NULL, "Failed to open directory!");

    return temp;
error:
    return NULL;
}

inline int Directory_getDirEnt(Directory *dir, DArray *entries){
    check(dir != NULL, "Invalid Directory Object!");
    check(entries != NULL, "Invalid entries DArray!");

    struct dirent *items = NULL;
    bstring entry = NULL; 

    while((items = readdir(dir->dirptr)) != NULL){
        bstring entry = bfromcstr(items->d_name);
        check(entry != NULL, "Failed to directory entries!");
        check(DArray_push(entries, entry) == 0, "Failed to add an entry in entries DArray!");
    }

    return 0;
error:
    if(entry) bdestroy(entry);
    return -1;
}

void Directory_close(Directory *dir){
    if(dir){
        if(dir->dir){
            Path_close(dir->dir);
            dir->dir = NULL;
        }
        if(dir->dirptr){
            closedir(dir->dirptr);
            dir->dirptr = NULL;
        }
        free(dir);
    }
}
