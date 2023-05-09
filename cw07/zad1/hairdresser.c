#include "headers.h"
#include "sem.h"
#include "shared_memory.h"
#include "haircut_queue.h"

void open_sems();
int open_client_mutex(int client_pid);


int main(){
    open_sems();
    Queue * client_queue = attach_shared_queue(SHARED_MEMORY);

    int client_mutex = -1;
    printf("[hairdresser %d] Ehh another day at this stupid job\n", getpid());

    while(true){
        acquire(sem_haircuts, 0);
        printf("[hairdresser %d] Oh god, this guy looks like he hasn't been sheared for ages!\n", getpid());
        acquire(sem_chairs, 0);
        printf("[hairdresser %d] ~ to client ~ : take a seat and relax\n", getpid());
        // sprawdź czy ktoś jest w kolejce
        release(sem_wait, 0);

        acquire(sem_queue_mutex, 0);
        haircut * new_haircut = pop(client_queue);
        if(new_haircut == NULL){
            printf("NULL HAIR\n");
        }
        printf("[hairdresser %d] 'jeeez, he will look terribly with this haircut' \n", getpid());
        release(sem_queue_mutex, 0);
        
        client_mutex = open_client_mutex(new_haircut->client_id);

        sleep(new_haircut->hiarcut_type);
        free(new_haircut);
        release(sem_chairs, 0);
        printf("[hairdresser %d] 'Finally done' \n", getpid());

        release(client_mutex, 0);

        release(sem_hairdresser, 0); 

        if (queue_empty(client_queue))
        {
            printf("I hope nobody more shows up...\n");
            sleep(TIMEOUT);
            if (queue_empty(client_queue))
                break;
        }
    }
    printf("FINALLY! I can go home now and get some sleep...\n");
    return 0;
}

int open_client_mutex(int client_pid){
    int key;
    if((key = ftok(MUTEX_PATH, client_pid)) == -1){
        perror("Failed to create semaphore: key");
        exit(EXIT_FAILURE);
    }
    int semid;
    if((semid = semget(key, 0, 0)) == -1){
        perror("Failed to create semaphore: semget");
        exit(EXIT_FAILURE);
    }
    return semid;
}

void open_sems(){
    sem_hairdresser = open_sem(SEM_HAIRDRESSER_NAME);
    sem_chairs = open_sem(SEM_CHAIRS_NAME);
    sem_wait = open_sem(SEM_WAIT_NAME);
    sem_haircuts = open_sem(SEM_HAIRCUTS_NAME);
    sem_queue_mutex = open_sem(SEM_QUEUE_MUTEX_NAME);
}
