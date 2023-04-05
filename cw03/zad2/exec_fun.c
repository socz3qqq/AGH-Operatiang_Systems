#include "exec_fun.h"

int main(int argc, char * argv[]){
    char * dir_path;
    // check the data  
    if ( argc != 2){
        fprintf(stderr, "Invalid number of arguments, please enter only one argument - directory path\n");
        return 2;
    }
    if(directoryExists(argv[1]))
        dir_path = argv[1];
    else{
        fprintf(stderr, "Invalid directory path: %s\n", argv[1]);
        return 2;
    }
    // print the directory path
    printf("%s: ", dir_path);
    fflush(stdout);
    // exeute ls command
    execl("/bin/ls", "ls", dir_path, NULL);
    
    // success!
    return 0;
}

boolean directoryExists(char * path){
    struct stat stats;

    stat(path, &stats);

    if(S_ISDIR(stats.st_mode))
        return true;
    return false;
}