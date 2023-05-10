#include "headers.h"
#include "sem.h"
#include "shared_memory.h"
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int M, N, P;

void handle_input(int argc, char * argv[]);
void create_semaphores();
void start_simulation();
void clear_up();
void spawn_hairdressers();
void spawn_clients();
void init_shared_queue();

int main(int argc, char * argv[]){
    handle_input(argc, argv);

    init_shared_queue();

    create_semaphores();
    start_simulation();

    while(wait(NULL) > 0);

    clear_up();

    printf("simulation finished\n");

    return 0;
}


void handle_input(int argc, char * argv[]){
    if( argc == 3){
        M = atoi(argv[1]);
        N = atoi(argv[2]);
    }
    else{
        printf("Invalid input!\n");
        exit(EXIT_FAILURE);
    }
}

void start_simulation(){
    printf("creating hairdressers...\n");
    spawn_hairdressers();
    printf("creating clients...\n");
    spawn_clients();
}

void create_semaphores(){
    printf("opened sem_hairdresser %d\n", create_sem(SEM_HAIRDRESSER_NAME, M));
    printf("opened sem_chairs %d\n", create_sem(SEM_CHAIRS_NAME, N));
    printf("opened sem_wait %d\n", create_sem(SEM_WAIT_NAME, WAITING_ROOM));
    printf("opened sem_haircuts %d\n", create_sem(SEM_HAIRCUTS_NAME, 0));
    printf("opened sem_queue_mutex%d\n", create_sem(SEM_QUEUE_MUTEX_NAME, 1));
}

void init_shared_queue(){
    int shmid = create_shared_queue(SHARED_MEMORY);
    if(shmid < 0){
        perror("shmget");
    }
    Queue * queue = (Queue*) shmat(shmid, NULL, 0);
    if(queue == (Queue*) -1){
        perror("shmat");
    }
    queue->head = 0;
    queue->tail = 0;
    queue->length = 0;
}

void spawn_hairdressers(){
    for ( int i = 0; i < M; i++){
        if(fork() == 0){
            if(execl(HAIRDRESSER, HAIRDRESSER, NULL) == -1){
                perror("hairdresser fork");
            }
        }
    }
}

void spawn_clients(){
for ( int i = 0; i < CLIENTS_NUMBER; i++){
        if(fork() == 0){
            if(execl(CLIENT, CLIENT, NULL) == -1){
                perror("client fork");
            }
        }
        sleep(1);
    }
}

void clear_up(){
    unlink_shared_queue(SHARED_MEMORY);
    unlink_sem(SEM_HAIRDRESSER_NAME);
    unlink_sem(SEM_CHAIRS_NAME);
    unlink_sem(SEM_WAIT_NAME);
    unlink_sem(SEM_HAIRCUTS_NAME);
    unlink_sem(SEM_QUEUE_MUTEX_NAME);
}