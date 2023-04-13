#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
// #include <linux/ipc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define QUEUE_NAME "."
#define HELLOW "Hello world!"
#define FRWARD "Cala naprzod!"

struct msgbuff{
    long mtype;
    char mtext[2048];
};

int main(){
    key_t key = ftok(QUEUE_NAME, '8');
    struct msgbuff hellow, forward, received;
    strcpy(hellow.mtext, HELLOW);
    hellow.mtype = 0;

    int msqid = msgget(key, 0664 | IPC_CREAT);

    msgsnd(msqid, &hellow, sizeof(HELLOW), IPC_NOWAIT);

    msgrcv(msqid, &received, sizeof(HELLOW), 0, IPC_NOWAIT);

    printf("%s\n", received.mtext);

    
}