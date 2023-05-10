#include "haircut_queue.h"

void push(Queue * queue, pid_t client){
    if(!queue_empty(queue)){
        queue->tail  = (queue->tail + 1)%MAX_QUEUE_SIZE;
    }
    queue->q[queue->tail] = client;
    queue->length += 1;
}

pid_t pop(Queue * queue){
    if(queue_empty(queue)){
        printf("Why you want to take something from an empty queue? pid: %d\n", getpid());
        return -1;
    }
    pid_t head_haircut = queue->q[queue->head];
    if(queue->length > 1) queue->head += 1;
    queue->length -= 1;

    return head_haircut;
}

bool queue_empty(Queue * queue){
    return queue->length == 0;
}