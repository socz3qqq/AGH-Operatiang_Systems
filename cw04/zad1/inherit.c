#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_ARG_SIZE 7
#define SIGNAL SIGUSR1


void handlerSIGNAL(int signum){
    printf("Obsługa sygnału %d\n", signum);
}

int main(int argc, char * argv[]){
    pid_t pid;
    char argument[MAX_ARG_SIZE];

    printf("This program demonstrates how the signal (SIGUSR1 is used) settings are inherited after executing fork and exec \nresults are stored in raport2.txt \n");

    if( argc != 2){
        fprintf(stderr, "Invalid number of arguments, please provide one argument:\n - ignore/handler/mask/pending\n");
        exit(1);
    }
    if(strcmp(argv[1], "ignore") == 0){
        printf("Option: ignore\n");
        printf("Parent: ");
        signal(SIGNAL, SIG_IGN);
        raise(SIGNAL);

        if ( (pid = fork()) < 0 ){  
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            printf("child: ");
            fflush(NULL);
            raise(SIGNAL);
        }
        printf("\n");
    }
    else if( strcmp(argv[1], "handler") == 0){
        printf("Option: handler\n");
        printf("Parent: ");
        signal(SIGNAL, &handlerSIGNAL);
        raise(SIGNAL);

        if ( (pid = fork()) < 0 ){
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            printf("child: ");
            raise(SIGNAL);
        }
        printf("\n");
    }
    else if( strcmp(argv[1], "mask") == 0){\
        printf("Option: mask\n");
        printf("Parent: ");
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGNAL);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("Failed to block new signal\n");
        raise(SIGNAL);
        if ( (pid = fork()) < 0 ){
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            printf("child: ");
            fflush(NULL);
            raise(SIGNAL);
        }
        printf("\n");
    }
    else if( strcmp(argv[1], "pending") == 0){
        printf("Option: pending\n");
        printf("Parent: ");
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGNAL);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("Failed to block new signal\n");
        raise(SIGNAL);
        if ( (pid = fork()) < 0 ){
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            sigset_t blocked_sigs;
            sigemptyset(&blocked_sigs);
            printf("child: \n");
            sigpending(&blocked_sigs);
            if(sigismember(&blocked_sigs, SIGNAL)){
                printf("Pending signal was copied to child process\n");
            }
            else{
                printf("Pending signal was NOT copied to child process\n");
            }
        }
        printf("\n");
    }
    else{
        fprintf(stderr, "Invalid argument, please provide one argument:\n - ignore/handler/mask/pending");
        exit(1);
    }



    if ( (pid = fork()) < 0 ){
        fprintf(stderr, "Failed to create child process\n");
        exit(1);
    }
    return 0;
}