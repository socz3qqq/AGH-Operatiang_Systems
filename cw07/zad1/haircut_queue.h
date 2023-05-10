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
#ifndef MAX_QUEUE_SIZE
#define MAX_QUEUE_SIZE 1024

#endif

typedef struct Queue{
    int head;
    int tail;
    int length;
    int q[MAX_QUEUE_SIZE];
} Queue;

void push(Queue * queue, pid_t client);

int pop(Queue * queue);

bool queue_empty(Queue * queue);

#endif