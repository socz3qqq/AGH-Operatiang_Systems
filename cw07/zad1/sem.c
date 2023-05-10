#include "sem.h"

int create_sem(const char * filename, int val){
    key_t key;
    int semid;
    if((key = ftok(getenv("HOME"), filename[0])) == -1){
        perror("Failed to create semaphore: key");
        return -1;
    }
    if((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1){
        perror("Failed to create semaphore: semget");
        return -1;
    }
    if(semctl(semid, 0, SETVAL, val) == -1){
        perror("Failed to set value: semctl");
        return -1;
    }
    return semid;
}
int open_sem(const char * filename){
    key_t key;
    int semid;
    if((key = ftok(getenv("HOME"), filename[0])) == -1){
        perror("Failed to open semaphore: key");
        return -1;
    }
    if((semid = semget(key, 1, 0)) == -1){
        printf("filename: %s, pid: %d\n", filename, getpid());
        perror("Failed to open semaphore: semget");
        return -1;
    }
    return semid;
}
void unlink_sem(const char * filename){
    int semid = open_sem(filename);
    if(semid == -1){
        perror("Failed to unlink semaphore: open_sem");
        return;
    }
    if(semctl(semid, 0, IPC_RMID) == -1){
        perror("Failed to unlink semaphore: semctl");
        return;
    }
}
int acquire(int semid, int flags){
    struct sembuf operations = { 0, -1, flags};
    return semop(semid, &operations, 1);

}
void release(int semid, int flags){
    struct sembuf operations = { 0, 1, flags};
    if(semop(semid, &operations, 1) == -1){
        perror("release ");
    }
}
