#ifndef __HEADERS_H__
#define __HEADERS_H__

#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define HOME getenv("HOME")
#define MSG_LENGTH 128
#define MAX_NO_CLIENTS 5

enum MsgType{
    INIT = 1,
    STOP = 6,
    LIST = 5,
    TO_ALL = 3,
    TO_ONE = 4,
    ACK = 2,
};

typedef enum { false, true} bool;

typedef struct MessageBuff{
    long mtype;
    char mtext[MSG_LENGTH];
    int sender_id;
    int send_to;

} MessageBuff;

const int MSG_SIZE = sizeof(struct MessageBuff);
#endif