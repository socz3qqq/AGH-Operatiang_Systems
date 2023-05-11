#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void * print_hello(void * thread_number){
    int state;
    sigset_t sigmask;
    sigfillset(&sigmask);
    sigdelset(&sigmask, SIGUSR1);
    sigdelset(&sigmask, SIGUSR2);
    pthread_sigmask(SIG_SETMASK, &sigmask, NULL);
    printf("hello I am thread %d\n", * (int * ) thread_number);

    printf("hello I am thread %d\n", * (int * ) thread_number);
    sleep(4);
    return thread_number;
}

int signal_handle(int signum){
    printf("OK, sygnał otrzymany\n");
}

int main(){
    int * arg = calloc(sizeof(int), 1), * th_ret;
    *arg = 5;

    sigset_t sigmask;
    sigfillset(&sigmask);

    struct sigaction sa;
    sa.sa_mask = sigmask;
    sa.sa_handler = signal_handle; 

    signal(SIGUSR1, signal_handle);

    pthread_t thread;
    pthread_create(&thread, NULL, &print_hello, (void *) arg);
    pthread_kill(thread, SIGUSR1);
    pthread_join(thread, & * (void *)th_ret);

    printf("received value %d\n", (int) * th_ret);
    return 0;
}