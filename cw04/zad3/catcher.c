
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

int mode_counter;
int current_mode;


void print_numbers(){
    for(int i = 0; i< 101; i++){
        printf("%d ", i);
    }
    printf("\n\n");
}

void print_curr_time(){
    time_t curr_time = time(NULL);
    char* time_string = ctime(&curr_time);
    printf("Current time: %s\n", time_string);
}

void print_mode_changes(){
    printf("Mode changes from the begging: %d\n\n", mode_counter);
}

void print_time_loop(){
    time_t curr_time = time(NULL);
    char* time_string = ctime(&curr_time);
    alarm(1);
    printf("Current time: %s\n", time_string);
}

void finish(int sender_pid){
    printf("Received 5, exiting...\n");
    kill(sender_pid, SIGUSR1);
    exit(0);
}

void handle_alarm(){
    if(current_mode == 4)
        print_time_loop();
}

void sigusr1_handler(int signum, siginfo_t * siginfo, void * f){
    printf("Received signal SIGUSR1 from : [pid] %d | [operation] : %d\n", siginfo->si_pid, siginfo->si_value.sival_int);
    mode_counter = mode_counter + 1;
    switch(siginfo->si_value.sival_int){
        case 1:
            current_mode = 1;
            print_numbers();
            break;
        case 2:
            current_mode = 2;
            print_curr_time();
            break;
        case 3:
            current_mode = 3;
            print_mode_changes();
            break;
        case 4:
            current_mode = 4;
            print_time_loop();
            break;
        case 5:
            current_mode = 5;
            finish(siginfo->si_pid);
        default:
            fprintf(stderr, "INVALID ARGUMENT\n");
    }
    kill(siginfo->si_pid, SIGUSR1);
    return;
}

int main(){
    struct sigaction sa, sa_alarm;
    sigfillset(&sa_alarm.sa_mask);
    sa_alarm.sa_handler = handle_alarm;
    sa_alarm.sa_flags &= ~SA_RESETHAND;
    sigfillset(&sa.sa_mask); // I decided to turn off all of the signals during handling SIGUSR1 signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sigusr1_handler;
    
    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset, SIGINT);
    sigdelset(&sigset, SIGUSR1);
    sigdelset(&sigset, SIGALRM);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa_alarm, NULL);
    printf("[CATCHER] : pid: %d\n", getpid());    
    while(1){
        sigsuspend(&sigset);
    }
}