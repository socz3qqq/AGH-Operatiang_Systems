#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG_SIZE 7
#define SIGNAL SIGUSR1


int main(int argc, char * argv[]){
    if( argc != 2){
        fprintf(stderr, "Invalid number of arguments, please provide one argument:\n - ignore/handler/mask/pending\n");
        exit(1);
    }
    if ( strcmp(argv[1], "pending") == 0){
        sigset_t blocked_sigs;
        sigemptyset(&blocked_sigs);
        sigpending(&blocked_sigs);
        if(sigismember(&blocked_sigs, SIGNAL)){
            printf("Signal is pending after exec\n");
        }
        else{
            printf("Signal is not pending after exec\n");
        }
    }
    else{
        printf("Raising signal after exec (pid: %d): ", getpid());
        fflush(NULL);
        raise(SIGUSR1);
    }
    printf("\n-------------------\n");
    return 0;
}