#ifndef __HAIRCUT_QUEUE_H__
#define __HAIRCUT_QUEUE_H__

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct haircut{
    int hiarcut_type;
    pid_t client_id;
    struct haircut* next;

} haircut;

typedef struct Queue{
    haircut* head;
    haircut* tail;
} Queue;

void push(Queue * queue, int type, pid_t client);

haircut * pop(Queue * queue);

bool queue_empty(Queue * queue);

#endif