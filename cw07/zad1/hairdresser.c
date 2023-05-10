#include "sem.h"
#include "shared_memory.h"
#include "haircut_queue.h"
#include "headers.h"

void open_sems();
int open_client_mutex(int client_pid);
bool waiting_room_empty();
bool hair_queue_empty();
void check_waiting_room();


int main(){
    open_sems();
    Queue * client_queue = attach_shared_queue(SHARED_MEMORY);

    int client_mutex = -1;
    printf("[hairdresser %d] Ehh another day at this stupid job\n", getpid());

    while(true){
        acquire(sem_haircuts, 0);
        printf("[hairdresser %d] Oh god, this guy looks like he hasn't been sheared for ages!\n", getpid());
        check_waiting_room();
        if(!waiting_room_empty()) release(sem_wait, 0);


        acquire(sem_queue_mutex, 0);
        pid_t new_haircut = pop(client_queue);
        if(new_haircut == -1){
            printf("BAD HAIR\n");
        }
        release(sem_queue_mutex, 0);

        acquire(sem_chairs, 0);
        printf("[hairdresser %d] ~ to client %d~ : take a seat and relax\n", getpid(), new_haircut);
        
        client_mutex = open_client_mutex(new_haircut);

        sleep((new_haircut+8)%15);
        release(sem_chairs, 0);
        printf("[hairdresser %d] 'Finally done' \n", getpid());

        release(client_mutex, 0);

        release(sem_hairdresser, 0); 

        if (waiting_room_empty() && hair_queue_empty())
        {
            printf("[hairdresser %d] I hope nobody more shows up...\n", getpid());
            sleep(TIMEOUT);
            if (waiting_room_empty() && hair_queue_empty())
                break;
        }
    }
    printf(" [hairdresser %d] FINALLY! I can go home now and get some sleep...\n", getpid());
    return 0;
}

int open_client_mutex(int client_pid){
    int key;
    if((key = ftok(MUTEX_PATH, client_pid)) == -1){
        perror("Failed to open semaphore: key");
        exit(EXIT_FAILURE);
    }
    int semid;
    if((semid = semget(key, 0, 0)) == -1){
        perror("Failed to open semaphore: semget");
        exit(EXIT_FAILURE);
    }
    return semid;
}
bool waiting_room_empty(){
    int value = semctl(sem_wait, 0, GETVAL);
    if(value==-1){
        perror("read sem_val fucked up");
    }
    return value == WAITING_ROOM;
}

bool hair_queue_empty(){
    return semctl(sem_haircuts, 0, GETVAL) == 0;
}

void check_waiting_room(){
    printf("waiting room: %d\n", semctl(sem_wait, 0, GETVAL));
}

void open_sems(){
    sem_hairdresser = open_sem(SEM_HAIRDRESSER_NAME);
    sem_chairs = open_sem(SEM_CHAIRS_NAME);
    sem_wait = open_sem(SEM_WAIT_NAME);
    sem_haircuts = open_sem(SEM_HAIRCUTS_NAME);
    sem_queue_mutex = open_sem(SEM_QUEUE_MUTEX_NAME);
}
