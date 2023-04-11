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


void handlerSIGNAL(int signum){
    printf("Received signal  %d\n", signum);
}

int main(int argc, char * argv[]){
    pid_t pid;
    int p = 1;
    if( argc != 2){
        fprintf(stderr, "Invalid number of arguments, please provide one argument:\n - ignore/handler/mask/pending\n");
        exit(1);
    }
    if(strcmp(argv[1], "ignore") == 0){
        printf("Option: ignore\n");
        signal(SIGNAL, SIG_IGN);
        printf("Raising signal in parent (pid: %d):\n", getpid());
        fflush(NULL);
        raise(SIGNAL);
        sleep(1);
        printf("\n");

        if ( (pid = fork()) < 0 ){  
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            printf("Raising signal in child (pid: %d):\n", getpid());
            fflush(NULL);
            raise(SIGNAL);
        }
        else{
            while((p = wait(NULL)) > 0){}
            execl("signal_for_exec", "signal_for_exec", "ignore", NULL);
        }
        printf("\n");
    }
    else if( strcmp(argv[1], "handler") == 0){
        printf("Option: handler\n");
        signal(SIGNAL, &handlerSIGNAL);
        printf("Raising signal in parent (pid: %d):\n", getpid());
        fflush(NULL);
        raise(SIGNAL);
        sleep(1);
        printf("\n");

        if ( (pid = fork()) < 0 ){
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            printf("Raising signal in child (pid: %d):\n", getpid());
            fflush(NULL);
            raise(SIGNAL);
        }
        else{
            while( (p = wait(NULL)) > 0);
            printf("\n --------------------\n");
        }
    }
    else if( strcmp(argv[1], "mask") == 0){\
        printf("Option: mask\n");
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGNAL);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("Failed to block new signal\n");
        raise(SIGNAL);
        printf("Raising signal in parent (pid: %d):\n", getpid());
        fflush(NULL);
        sleep(1);
        printf("\n");
        if ( (pid = fork()) < 0 ){
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            printf("Raising signal in child (pid: %d):\n", getpid());
            fflush(NULL);
            raise(SIGNAL);
        }
        else{
            while((p = wait(NULL)) > 0){}
            printf("\n");
            execl("signal_for_exec", "signal_for_exec","mask", NULL);
        }
    }
    else if( strcmp(argv[1], "pending") == 0){
        printf("Option: pending\n");
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGNAL);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("Failed to block new signal\n");
        raise(SIGNAL);
        sleep(1);
        printf("\n");
        if ( (pid = fork()) < 0 ){
            fprintf(stderr, "Failed to create child process\n");
            exit(1);
        }
        if(pid == 0){
            sigset_t blocked_sigs;
            sigemptyset(&blocked_sigs);
            sigpending(&blocked_sigs);
            if(sigismember(&blocked_sigs, SIGNAL)){
                printf("Pending signal was copied to child process\n");
            }
            else{
                printf("Pending signal was NOT copied to child process\n");
            }
        }
        else{
            while((p = wait(NULL)) > 0){}
            execl("signal_for_exec", "signal_for_exec", "pending", NULL);
        }
        if(pid > 0) printf("\n");
    }
    else{
        fprintf(stderr, "Invalid argument, please provide one argument:\n - ignore/handler/mask/pending");
        exit(1);
    }
    return 0;
}