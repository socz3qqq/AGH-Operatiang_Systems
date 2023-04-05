#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int counter = 0;

void handleSIGUSR1(int signum){
    counter += 1;
    printf("signal %d\n", signum);
}

int main(){

    pid_t pid = fork();
    if(pid == 0){
        printf("działa fork\n");
        signal(SIGINT, handleSIGUSR1);
    }
    printf("pid: %d\n", pid);
    if(pid > 0){
        printf("macierzysty\n");
        // sleep(1);
        kill(pid, SIGUSR1);
    }
    if(pid == 0){
        printf("potomny\n");
        sleep(10);
        // pause();
    }

    //  pid_t  p;
    // int    status;

    // while ((p = wait(&status)) > 0) {
    //     if (WIFEXITED(status))
    //         printf("Child %ld exit status was %d.\n", (long)p, WEXITSTATUS(status));
    //     else
    //     if (WIFSIGNALED(status))
    //         printf("Child %ld was killed by signal %d.\n", (long)p, WTERMSIG(status));
    //     else
    //         printf("Child %ld was lost.\n", (long)p);
    //     fflush(stdout);
    // }

    return 0;
}

// zmodyfikuj program w taki sposob w procesie potomnym zdefiniujesz zbior sygnałow blokowanych ustaw g o na pelny(sigset, sigfillset, sigprocmask

// przetestuj działanie zestwu progamow dla sugnalow ) sigkill sigabort)