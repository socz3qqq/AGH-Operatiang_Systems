#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

typedef enum { false, true } boolean;

boolean directoryExists(char * path){
    struct stat stats;

    stat(path, &stats);

    if(S_ISDIR(stats.st_mode))
        return true;
    return false;
}

int searchDirectoryRec(char * curr_dir_path, char * string){
    if (chdir(curr_dir_path) < 0){
        perror("ERROR");
        return -1;
    }

    DIR * curr_dir;
    struct dirent * dir_entity;
    struct stat file_stat;
    int pid;
    FILE * curr_file;

    char string_buff[255] = "";
    char curr_file_path[PATH_MAX];

    if(!directoryExists(curr_dir_path)){
        fprintf(stderr, "Nie znaleziono katalogu: %s\n", curr_dir_path);
        return -1;
    }
    if((curr_dir = opendir(curr_dir_path))==NULL){
        printf("dir: %s\n", curr_dir_path);
        perror("Error");
        return -1;
    }

    while((dir_entity = readdir(curr_dir)) != NULL){
        if(strcmp(dir_entity->d_name, ".") == 0 || strcmp(dir_entity->d_name, "..") == 0)
            continue;
        // printf("%s\n", dir_entity->d_name);
        if(stat(dir_entity->d_name, &file_stat) < 0){
            perror("Error");
            continue;
        }
        if(S_ISDIR(file_stat.st_mode)){
            // printf("Hey i found a directory: %s\n", dir_entity->d_name);
            pid = fork();
            if(pid == 0){
                if(getcwd(curr_dir_path, 255) == NULL){
                    perror("ERROR");
                    ;
                }
                strcat(curr_dir_path, "/");
                strcat(curr_dir_path, dir_entity->d_name);
                searchDirectoryRec(curr_dir_path, string);
                break;
            }
        }
        else{
            if( (curr_file = fopen(dir_entity->d_name, "r")) == NULL){
                perror("ERROR");
                continue;
            }
            if((fread(string_buff, sizeof(char), strlen(string), curr_file)) == 0){
                ferror(stderr);
                continue;
            }
            if(strcmp(string_buff, string) == 0){
                if(getcwd(curr_dir_path, 255) == NULL){
                    fclose(curr_file);
                    perror("ERROR");
                    return -1;
                }
                strcpy(curr_file_path, curr_dir_path);
                strcat(curr_file_path, "/");
                strcat(curr_file_path, dir_entity->d_name);
                printf("file path: %s, file id: %ld \n", curr_file_path, dir_entity->d_ino);
                strcpy(curr_file_path, "");
                fflush(NULL);
            }
            fclose(curr_file);
        }
    }
    closedir(curr_dir);
    
    return 0;
}

int main(int argc, char * argv[]){
    if ( argc != 3){
        fprintf(stderr, "ERROR: Invalid number of arguments, please enter 2 arguments:\n  1. directory path\n  2. string no longer than 255 chars\n");
        return 2;
    }
    if(strlen(argv[2]) > 255){
        fprintf(stderr, "ERROR: string is too long: %s\n", argv[2]);
        return 2;
    }
    if (chdir(argv[1]) < 0){
        perror("ERROR");
        return -1;
    }
    char curr_dir_path[PATH_MAX];
    if(getcwd(curr_dir_path, 255) == NULL){
        perror("ERROR");
        return -1;
    }
    
    searchDirectoryRec(curr_dir_path, argv[2]);

    while (wait(NULL) > 0);

    return 0;
}