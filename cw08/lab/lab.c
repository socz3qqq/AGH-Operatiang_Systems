#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 64

pthread_t threads[MAX_THREADS];

static void * print_hello(void * thread_number){
    int state;
    printf("hello I am thread %d\n", * (int * ) thread_number);
    pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, &state);
    pthread_testcancel();
    pthread_setcancelstate(PTHREAD_CANCEL_DEFERRED, &state);

    printf("hello I am thread %d\n", * (int * ) thread_number);
    free(thread_number);
    return 0;
}

int main(int argc, char * argv[]){
    if(argc != 2){
        printf("invalid arguments");
        return 0;
    }
    int n = atoi(argv[1]), th_ret, m = 0;
    while(1){
        for(int i = 0; i < n; i++){
            int * arg = calloc(sizeof(int), 1);
            *arg = i;
            th_ret = pthread_create(&threads[i], NULL, &print_hello, arg);
        }
        printf("hello to proces główny, iteracja %d\n", m);
        sleep(1);
        m += 1;

        if(m == 10){
            for(int j = 0; j < MAX_THREADS; j ++){
                if(threads[j] > 0){
                    pthread_cancel(threads[j]);
                    return 0;
                }
            }
        }
    }

    return 0;
}

