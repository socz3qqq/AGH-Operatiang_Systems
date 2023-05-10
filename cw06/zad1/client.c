#include "headers.h"

int client_id, server_id;
bool connected = false;
MessageBuff msg_buff;


void sigint_handler(int signum){
    printf("\nsignum: %d\n", signum);
    if(connected){
        MessageBuff msg = { .mtype = STOP, .send_to = server_id, .sender_id=client_id};
        if(msgsnd(server_id, &msg, sizeof(MessageBuff), 0) == -1){
            perror("msgsnd");
        }
    }
    msgctl(client_id, IPC_RMID, NULL);
    exit(0);
}

void react_to_server_messages(){
    if(msgrcv(client_id, &msg_buff, sizeof(MessageBuff), STOP, IPC_NOWAIT) == -1){}
    else{
        connected = false;
        msg_buff.send_to = server_id;
        msg_buff.sender_id = client_id;
        msg_buff.mtype = STOP;
        strcpy(msg_buff.mtext, "ack");
        printf("sending ack of connection end\n");
        if(msgsnd(server_id, &msg_buff, sizeof(MessageBuff), 0) == -1){
            perror("msgsnd");
        }
        server_id = -1;
        return;
    }
    while(msgrcv(client_id, &msg_buff, sizeof(MessageBuff), 0, IPC_NOWAIT) != -1)
        printf("Message from %d: %s\n", msg_buff.sender_id, msg_buff.mtext);

}

int main(){
    srand(time(NULL));

    key_t ipc_key = ftok(HOME, rand() % 255 + 1);

    client_id = msgget(ipc_key, IPC_CREAT | 0666);

    key_t sKey = ftok(HOME, 1);
    server_id = msgget(sKey, 0);

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigfillset(&sa.sa_mask);
    // sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    size_t len = 0;
    size_t read;
    char * cmd = NULL;
    char * command = NULL;

    printf("Client started!\n client id: %d\n", client_id);

    while(1){

        react_to_server_messages();

        printf("User command: ");
        read = getline(&cmd, &len, stdin);
        cmd[read - 1] = '\0';
        fflush(NULL);

        if(strcmp(cmd, "") == 0){
            continue;
        }

        command = strtok(cmd, " ");

        if( !connected && strcmp(command, "INIT") != 0){
            printf("\nPlease connect to a server using: INIT <server_id>\n");
            continue;
        }
        else if(!connected && strcmp(command, "INIT") == 0){
            printf("init\n");
            command = strtok(NULL, " ");
            server_id = atoi(command);
            MessageBuff msg = { .mtype = INIT, .send_to = server_id, .sender_id=client_id};
            strcpy(msg.mtext, "Hello server, will you let me in?");
            // printf("sending this message: sender: %d, sen_to: %d, msg: %s, type: %ld\n", msg.sender_id, msg.send_to, msg.mtext, msg.mtype);
            
            if(msgsnd(server_id, &msg, sizeof(MessageBuff), 0) == -1){
                perror("msgsnd");
                continue;
            
            }
            printf("waiting for response...\n");
            if(msgrcv(client_id, &msg, sizeof(MessageBuff), INIT, 0) == -1){
                perror("msgrcv");
            }
            printf("response: %s\n", msg.mtext);
            if(strcmp(msg.mtext, "Connection refused: max number of clients reached") != 0)
                connected = true;
        }
        else if(strcmp(command, "STOP") == 0){
            // handle disconnect
            sigint_handler(SIGINT);
            printf("Disconnected from the server...\n");
            connected = false;
        }
        else if(strcmp(command, "LIST") == 0){
            //  handle list
            msg_buff.mtype = LIST;
            msg_buff.sender_id = client_id;
            msg_buff.send_to = server_id;
            strcpy(msg_buff.mtext, "Give me all active clients");
            if(msgsnd(server_id, &msg_buff, sizeof(MessageBuff), 0) == -1){
                perror("msgsnd");
                continue;
            }
            printf("waiting for response...\n");
            if(msgrcv(client_id, &msg_buff, sizeof(MessageBuff), LIST, 0) == -1){
                perror("msgrcv");
            }
            printf("Active clients: %s\n", msg_buff.mtext);
        }
        else if(strcmp(command, "2ALL") == 0){
            // handle sending messages to all
            msg_buff.mtype = TO_ALL;
            msg_buff.sender_id = client_id;
            msg_buff.send_to = server_id;
            // get the message
            command = strtok(NULL, " ");
            strcpy(msg_buff.mtext, command);
            if(msgsnd(server_id, &msg_buff, sizeof(MessageBuff), 0) == -1){
                perror("msgsnd");
                continue;
            }
        }
        else if(strcmp(command, "2ONE") == 0){
            // handle senging a message
            msg_buff.mtype = TO_ONE;
            msg_buff.sender_id = client_id;
            // get the id
            command = strtok(NULL, " ");
            msg_buff.send_to = atoi(command);
            command = strtok(NULL, " ");
            strcpy(msg_buff.mtext, command);
            if(msgsnd(server_id, &msg_buff, sizeof(MessageBuff), 0) == -1){
                perror("msgsnd");
                continue;
            }
        }
        else{
            fprintf(stderr, "Invalid command!\n");
            continue;
        }
    }

    free(command);
    
    return 0;
}