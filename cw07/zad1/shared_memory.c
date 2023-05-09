#include "shared_memory.h"
#include <errno.h>
#include <unistd.h>

int create_shared_queue(char * pathname){
    key_t key = ftok(getenv("HOME"), pathname[0]);
    if (key == -1){
        perror("Failed to create shared queue: ftok");
        return -1;
    }
    return shmget(key, sizeof(Queue),  0664 | IPC_CREAT | IPC_EXCL);
}
Queue * attach_shared_queue(char * pathname){
    key_t key = ftok(getenv("HOME"), pathname[0]);
    if (key == -1){
        perror("Failed to attach shared queue: ftok");
        return NULL;
    }
    int shmid = shmget(key, 0, 0);
    Queue * q = (Queue*) shmat(shmid, NULL, 0);
    if(*(int*) q == -1){
        perror("Failed to attach shared queue: shmat");
        return NULL;
    }
    return q;
}
int detach_shared_queue(Queue * q){
    return shmdt((const void *) q);
}

void unlink_shared_queue(char * pathname){
    key_t key = ftok(getenv("HOME"), pathname[0]);
    if (key == -1){
        perror("Failed to unlink shared queue: ftok");
        return;
    }
    int shmid = shmget(key, 0, 0);
    if(shmid == -1){
        perror("Failed to unlink shared queue: shmget");
    }
    shmctl(shmid, IPC_RMID, NULL);
}

