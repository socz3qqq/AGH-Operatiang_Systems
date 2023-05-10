#include "headers.h"
#include "sem.h"
#include "shared_memory.h"

int M, N, P;
int self_mutex;

void open_sems();
void create_self_mutex();
void delete_self_mutex();

int main(){ 
    printf("[CLIENT %d] hello there!\n", getpid());
    open_sems();
    create_self_mutex();
    Queue * client_queue = attach_shared_queue(SHARED_MEMORY);
    
    if(acquire(sem_hairdresser, IPC_NOWAIT) == -1){
        printf("[CLIENT %d] All hairdressers are accupied, I'll wait in the queue\n", getpid());
        if(acquire(sem_wait, IPC_NOWAIT) == -1){
            printf("[CLIENT %d] Queue is full, I will come another time\n", getpid());
            exit(0);
        }
         acquire(sem_hairdresser, 0);
    }
    printf("[CLIENT %d] Great! One of the hairdressers is free, I must choose a haircut now\n", getpid());
    if(acquire(sem_queue_mutex, 0) == -1){
        perror("failed to acquire queue\n");
    }
    push(client_queue, getpid());
  
    release(sem_queue_mutex, 0);

    release(sem_haircuts, 0);

    printf("[CLIENT %d] Hey barber please do not hurry, the haircut must be perfect!\n", getpid());
    acquire(self_mutex, 0);

    printf("[CLIENT %d] ~ walks out with a smile on his face...\n", getpid());

    delete_self_mutex();

    return 0;
}

void create_self_mutex(){
    int key;
    if((key = ftok(MUTEX_PATH, getpid())) == -1){
        perror("Failed to create semaphore: key");
        exit(EXIT_FAILURE);
    }
    if((self_mutex = semget(key, 1, 0664 | IPC_CREAT)) == -1){
        perror("Failed to create semaphore: semget");
        exit(EXIT_FAILURE);
    }
    if(semctl(self_mutex, 0, SETVAL, 0) == -1){
        perror("Failed to set value: semctl");
        exit(EXIT_FAILURE);
    }
}

void delete_self_mutex(){
    if(semctl(self_mutex, 0, IPC_RMID) == -1){
        perror("Failed to unlink semaphore: semctl");
        return;
    }
}

void open_sems(){
    sem_hairdresser = open_sem(SEM_HAIRDRESSER_NAME);
    sem_chairs = open_sem(SEM_CHAIRS_NAME);
    sem_wait = open_sem(SEM_WAIT_NAME);
    sem_haircuts = open_sem(SEM_HAIRCUTS_NAME);
    sem_queue_mutex = open_sem(SEM_QUEUE_MUTEX_NAME);
    // printf("client %d, opened sem_hairdresser %d\n", getpid(), sem_hairdresser);
    // printf("client %d, opened sem_chair %d\n", getpid(), sem_chairs);
    // printf("client %d, opened sem_wait %d\n", getpid(), sem_wait);
    // printf("client %d, opened sem_haircuts %d\n", getpid(), sem_haircuts);
    // printf("client %d, opened sem_queue_mutex%d\n", getpid(), sem_queue_mutex);
}

