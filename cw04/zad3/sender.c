#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

void sigusr1_handler(int signum){
    printf("Sender received confirmation\n");
    return;
}


int main(int argc, char* argv[] ){
    
    if( argc < 3){
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    struct sigaction sa;
    sigfillset(&sa.sa_mask); // I decided to turn off all of the signals during handling SIGUSR1 signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = sigusr1_handler;
    
    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset, SIGUSR1);
    sigdelset(&sa.sa_mask, SIGINT);
    
    sigaction(SIGUSR1, &sa, NULL);

    union sigval val;

    for(int i = 2; i < argc; i++){
        val.sival_int = atoi(argv[i]);
        sigqueue(atoi(argv[1]), SIGUSR1, val);
        sigsuspend(&sigset);
    }
}