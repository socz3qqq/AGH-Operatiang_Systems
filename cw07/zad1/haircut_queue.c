#include "haircut_queue.h"

void push(Queue * queue, int type, pid_t client){
    printf("pushing %d to the queue...\n", type);
    haircut * new_haircut = (haircut*) malloc(sizeof(haircut));
    if(new_haircut == NULL){
        perror("malloc");
    }

    new_haircut->hiarcut_type = type;
    new_haircut->client_id = client;
    new_haircut->next = NULL;
    if(queue->tail == NULL){
        queue->head = new_haircut;
        queue->tail = new_haircut;
    }else{
        queue->tail->next = new_haircut;
        queue->tail = new_haircut;
    }
    printf("tail: %d, head: %d\n", queue->tail->client_id, queue->head->client_id);
    printf("okay, pushed\n");
}

haircut * pop(Queue * queue){
    if(queue->head == NULL){
        return NULL;
    }
    printf("popping it\n");
    haircut * head_haircut = queue->head;
    if(head_haircut < (haircut*)sbrk(0)){
        printf("Huoston we've got a problem...\n");
    }
    printf("i have the value: %d\n", head_haircut->client_id);
    queue->head = queue->head->next;
    printf("checking if empty\n");
    if(queue->head == NULL){
        queue->tail = NULL;
    }
    printf("popped");
    return head_haircut;
}

bool queue_empty(Queue * queue){
    return queue->head == NULL;
}