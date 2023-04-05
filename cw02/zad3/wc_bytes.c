#include "wc_bytes.h"

int main(){
    // declaring variables
    DIR * directory;
    char cwd[CWD_BUFF_SIZE];
    struct stat file_stat_buff;
    int file_status;
    struct dirent * dir_entity;
    long long total_bytes = 0;
    // getting current working directory, could also just use "."
    getcwd(cwd, CWD_BUFF_SIZE);
    // opening a directory
    if ( (directory = opendir(cwd)) == NULL){
        fprintf(stderr, "Failed to open current working directory.\n");
        return 1;
    }
    while( (dir_entity = readdir(directory)) != NULL){
        if( (file_status = stat(dir_entity->d_name, &file_stat_buff)) == -1){
            fprintf(stderr, "Failed to get file info: %s, skipping... \n", dir_entity->d_name);
            continue;
        }
        if(!S_ISDIR(file_stat_buff.st_mode)){
            printf("size: %ld filename: %s\n", file_stat_buff.st_size,dir_entity->d_name);
            total_bytes += file_stat_buff.st_size;
        }
    }
    printf("total size of all files: %lld \n", total_bytes);
    if(closedir(directory) == -1){
        fprintf(stderr, "Failed to close current working directory.\n");
        return 1;
    }
    return 0;
}