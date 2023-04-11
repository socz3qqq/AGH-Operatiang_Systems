#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUFF_SIZE 50000

// pretty output to a console using escape sequences
#define TEXT_GREEN_BOLD "\033[1;32m"
#define TEXT_BOLD "\033[1;30m"
#define TEXT_ERROR "\033[3;31m"
#define TEXT_ITALIC "\033[3;30m"
#define TEXT_CLEAR "\033[0m"
#define TEXT_ITALIC_RED "\033[3;31m"

int signals_received;

void sig_info_handler(int signum, siginfo_t * sig_info, void *f){
    signals_received ++;
    printf(TEXT_BOLD "  si_signo : %d" TEXT_CLEAR, sig_info->si_signo);
    printf(TEXT_ITALIC "        - shows the signal number\n" TEXT_CLEAR);
    printf(TEXT_BOLD "  si_code : %d" TEXT_CLEAR, sig_info->si_code);
    printf(TEXT_ITALIC "        - indicates why this signal was sent\n" TEXT_CLEAR);
    printf(TEXT_BOLD "  si_pid : %d" TEXT_CLEAR, sig_info->si_pid);
    printf(TEXT_ITALIC "        - sender process id\n" TEXT_CLEAR);
    printf(TEXT_BOLD "  si_uid : %d" TEXT_CLEAR, sig_info->si_uid);
    printf(TEXT_ITALIC "        - sender user id\n" TEXT_CLEAR);
    printf(TEXT_BOLD "  si_status : %d" TEXT_CLEAR, sig_info->si_status);
    printf(TEXT_ITALIC "        - signal status\n" TEXT_CLEAR);
    if (sig_info->si_code == SI_QUEUE) {
        printf(TEXT_BOLD "  si_value : %d" TEXT_CLEAR, *(int*)sig_info->si_value.sival_ptr);
    } else {
        printf( TEXT_BOLD "  si_value : %p" TEXT_CLEAR, sig_info->si_value.sival_ptr);
    }
    printf(TEXT_ITALIC "        - signal value\n" TEXT_CLEAR);
    printf("\n");
    return;
}

void resethand_handler(int signum){
    printf(TEXT_ITALIC_RED"  ~Custom handler for signal: %d\n" TEXT_CLEAR, signum);
    return;
}

void nocldstop_handler(int signum){
    printf(TEXT_ITALIC_RED "Hey I am a child process, and I just finished my job :)\n" TEXT_CLEAR);
    return;
}

void check_disposition(int signum, struct sigaction sa){
    if (sa.sa_handler == resethand_handler) {
        printf(TEXT_BOLD "Handler for signal %d is set to: resethand_handler\n" TEXT_CLEAR, signum);
    }
    else if (sa.sa_handler == SIG_DFL) {
        printf(TEXT_BOLD "Handler for signal %d is set to: SIG_DFL\n" TEXT_CLEAR, signum);
    }
    else if (sa.sa_handler == SIG_IGN) {
        printf(TEXT_BOLD "Handler for signal %d is set to: SIG_IGN\n" TEXT_CLEAR, signum);
    }
    else {
        printf(TEXT_BOLD "Handler of signal %d is set to: unknown\n" TEXT_CLEAR, signum);
    }
}

void test_siginfo(struct sigaction sa){
    sa.sa_sigaction = sig_info_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    int pid, p;

    printf(TEXT_ITALIC "si_code examples:\n");
    printf("    -6 : SI_TKILL  - sent by tkill function\n");
    printf("    -1 : SI_QUEUE - sent by sigqueue function\n");
    printf("     0 : SI_USER  - sent by user using kill function\n");
    printf(TEXT_CLEAR);
    
    struct sigaction osa_sigusr1;
    struct sigaction osa_sigint;
    struct sigaction osa_sigchld;

    sigaction(SIGUSR1, &sa, &osa_sigusr1);
    sigaction(SIGINT, &sa, &osa_sigint);
    sigaction(SIGCHLD, &sa, &osa_sigchld);

    printf(TEXT_GREEN_BOLD "SHOW INFO [SIGUSR1]:\n" TEXT_CLEAR);
    raise(SIGUSR1);

    printf(TEXT_GREEN_BOLD "ANOTHER SIGNAL USING SIGQUEUE [SIGINT]:\n" TEXT_CLEAR);
    union sigval val;
    int * num = malloc(sizeof(int));
    *num = 15;
    val.sival_ptr = num;
    sigqueue(getpid(), SIGINT, val);

    printf(TEXT_GREEN_BOLD "CHILD PROCESS [SIGUSR1]:\n" TEXT_CLEAR);

    if((pid = fork()) < 0){
        perror("\033[0;31m" "ok");
    }
    if(pid == 0){
        signals_received = 0;
        printf("Child process sending signal to itself: \n");
        raise(SIGUSR1);
        while(signals_received < 2){
            pause();
        }
        exit(123);
    }
    else{
        sleep(1);
        printf("Parent process sending signal to child: \n");
        kill(pid, SIGUSR1);
        while((p = wait(NULL)) > 0){};

        printf("In the above example signal code is 1 which for the SIGCHLD signal means that child process exited\n \
and the si_status is the status witch, which it exited\n");
        printf("Additionally during this excersise I found out that raise uses tkill to send a signal\n");
    }
    free(num);
    // restore previous signal dispositions
    sigaction(SIGUSR1, &osa_sigusr1, NULL);
    sigaction(SIGINT, &osa_sigint, NULL);
    sigaction(SIGCHLD, &osa_sigchld, NULL);
}

void test_resethand(struct sigaction sa){
    sa.sa_handler = resethand_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESETHAND;
    check_disposition(SIGINT, sa);
    sigaction(SIGINT, &sa, NULL);
    printf(TEXT_ITALIC "First signal sent...\n" TEXT_CLEAR);
    raise(SIGINT);
    sleep(1);
    if (sigaction(SIGINT, NULL, &sa) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Check the current disposition of SIGINT
    check_disposition(SIGINT, sa);
    sleep(1);
    printf(TEXT_ITALIC "Second signal sent...\n" TEXT_CLEAR);
    raise(SIGINT);
}

void test_nocldstop(struct sigaction sa){

    printf(TEXT_ITALIC "\nAs we can see below the handler is called every time a SIGSTOP is called:\n" TEXT_CLEAR);

    struct sigaction osa_sigchld;

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = nocldstop_handler;
    sigaction(SIGCHLD, &sa, &osa_sigchld);
    int child_pids[10];
    for(int i = 0; i <5; i++){
        int wstatus;
        if((child_pids[i] = fork()) < 0){
            perror("signal");
            exit(EXIT_FAILURE);
        }
        if(child_pids[i] == 0){
            sleep(1);
            raise(SIGSTOP);
            printf("I am awake!\n");
            exit(100 + i);
        }
        else{
            sleep(1);
            printf("sending signal SIGKILL to pid: %d\n", child_pids[i]);
            waitpid(child_pids[i], &wstatus, WUNTRACED);
            kill(child_pids[i], SIGKILL);

        }
    }
    while(wait(NULL) > 0){};

    printf(TEXT_ITALIC "\nAnd now after setting SA_NOCLDSTOP flag, SIGSTOP is not sent in child process thus handler is called only once:\n" TEXT_CLEAR);

    sa.sa_flags = SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = nocldstop_handler;
    sigaction(SIGCHLD, &sa, NULL);
    
    for(int i = 5; i <10; i++){
        int wstatus;
        if((child_pids[i] = fork()) < 0){
            perror("signal");
            exit(EXIT_FAILURE);
        }
        if(child_pids[i] == 0){
            sleep(1);
            raise(SIGSTOP);
            printf("I am awake!\n");
            exit(100 + i);
        }
        else{
            sleep(1);
            printf("sending signal SIGKILL to pid: %d\n", child_pids[i]);
            waitpid(child_pids[i], &wstatus, WUNTRACED);
            kill(child_pids[i], SIGKILL);

        }
    }

    while(wait(NULL) > 0){};
    sigaction(SIGCHLD, &osa_sigchld, NULL);
}

int main(){

    struct sigaction sa;

    printf(TEXT_GREEN_BOLD "\n-----Testing SA_SIGINFO FLAG-----\n" TEXT_CLEAR);
    test_siginfo(sa);
    printf("\n\n");

    printf(TEXT_GREEN_BOLD "-----Testing SA_NOCLDSTOP FLAG-----\n" TEXT_CLEAR); 
    test_nocldstop(sa);
    printf("\n\n");

    printf(TEXT_GREEN_BOLD "-----Testing SA_RESETHAND FLAG-----\n" TEXT_CLEAR); 
    test_resethand(sa);
    printf("\n\n");

    return 0;
}
