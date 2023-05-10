#ifndef __HEADER_H__
#define __HEADER_H__
#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SERVER_QUEUE "/server_queue"
#define MAX_MSG_SIZE 128
#define MAX_NO_CLIENTS 4
#define NAME_SIZE 16

typedef enum bool{
    false, true
} bool;

typedef enum MsgType{
    ERR = 0,
    STOP = 5,
    LIST = 4,
    TO_ALL = 3,
    TO_ONE = 2,
    INIT = 1
}MsgType;

typedef struct message{
    char sender_name[NAME_SIZE];
    char recipient[NAME_SIZE];
    char msg[MAX_MSG_SIZE];

}Message;

const size_t MSG_BUFF_SIZE = sizeof(Message);

#endif