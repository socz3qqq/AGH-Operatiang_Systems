#include "headers.h"

int client_list[MAX_NO_CLIENTS];
int server_id;

void log_message(MessageBuff * message){
    char * my_time_string;
    time_t my_time = time(NULL);


    my_time_string= ctime(&my_time);   

    FILE * server_logs = fopen("server.log", "a");

    fprintf(server_logs, "%s, from: %d, to %d, message: %s\n\n ", my_time_string, message->sender_id, message->send_to, message->mtext);

    fclose(server_logs);
}

void react_to_communicate(MessageBuff * message){
    switch(message->mtype){
            case INIT:
                printf("handling init on server\n");
                MessageBuff msg_init = { .mtype = INIT, .send_to = message->sender_id, .sender_id = server_id};
                bool connection_established = false;
                for(int i = 0; i<MAX_NO_CLIENTS; i++){
                    if(client_list[i] == 0){
                        client_list[i] = message->sender_id;
                        connection_established = true;
                        strcpy(msg_init.mtext, "Succesfully connected ;D");

                        printf("sending confirmation to %d ...\n", client_list[i]);

                        if(msgsnd(client_list[i], &msg_init, sizeof(MessageBuff), 0) == -1){
                            perror("msgsnd");
                        }
                        printf("done\n");
                        break;
                    }
                }
                if(!connection_established){
                    strcpy(msg_init.mtext, "Connection refused: max number of clients reached");
                    msgsnd(message->sender_id, &msg_init, sizeof(MessageBuff), 0);
                }
                break;
            case STOP:
                for(int i = 0; i< MAX_NO_CLIENTS; i++){
                    if(client_list[i] == message->sender_id){
                        client_list[i] = 0;
                        break;
                    }
                }
                printf("Client %d disconnected\n", message->sender_id);
                break;
            case LIST:
                char msg_text[MSG_LENGTH];
                char snum[11];
                for( int i = 0; i < MAX_NO_CLIENTS; i++){
                    if(client_list[i] > 0){
                        sprintf(snum, "%d", client_list[i]);
                        strcat(msg_text, snum);
                        strcat(msg_text, ", ");
                    }
                }
                MessageBuff msg = { .mtype = LIST, .send_to = message->sender_id, .sender_id = server_id};
                strcpy(msg.mtext,  msg_text);
                if(msgsnd(message->sender_id, &msg, sizeof(MessageBuff), 0)==-1){
                    perror("msgsnd list");
                }
                break;
            case TO_ALL:
                for(int i = 0; i<MAX_NO_CLIENTS; i++){
                    if(client_list[i] > 0 && client_list[i] != message->sender_id){
                        message->send_to = client_list[i];
                        if(msgsnd(client_list[i], message, sizeof(MessageBuff), 0)== -1){
                            perror("msgsnd");
                        }
                        log_message(message);
                    }
                }
                break;
            case TO_ONE:
                if(msgsnd(message->send_to, message, sizeof(MessageBuff), 0) == -1){
                    perror("msgsnd");
                }
                break;
            default:
                MessageBuff default_msg;
                default_msg.mtype = 0;
                strcpy( default_msg.mtext,"Invalid command");
                default_msg.send_to = message->sender_id;
                default_msg.sender_id = server_id;
                
                msgsnd(message->sender_id, &default_msg, sizeof(MessageBuff), 0);
                break;
        }
}


void sigint_handler(int signum){
    printf("Disconnecting clients..\n");
    MessageBuff  msg_curr_dc;
    while(msgrcv(server_id, &msg_curr_dc, sizeof(MessageBuff), STOP, IPC_NOWAIT) != -1){
        react_to_communicate(&msg_curr_dc);
    }
    for( int i = 0; i < MAX_NO_CLIENTS; i++){
        if(client_list[i] > 0){
            MessageBuff  msg_stop = { .mtype = STOP, .send_to = client_list[i], .sender_id = server_id};
            strcpy(msg_stop.mtext, "Closing connection... ");
            if(msgsnd(client_list[i], &msg_stop, sizeof(MessageBuff), 0) ==-1){
                perror("msgsnd");
            }
            printf("waiting for response for id: %d\n", client_list[i]);
            if(msgrcv(server_id, &msg_stop, sizeof(MessageBuff), STOP, 0) == -1){
                perror("msgrcv");
            }
            react_to_communicate(&msg_stop);
        }
    }
    printf("Closing server... \n");
    msgctl(server_id, IPC_RMID, NULL);
    exit(0);
}

int main(){
    key_t ipc_key = ftok(HOME,  1);
    MessageBuff message;

    server_id = msgget(ipc_key, IPC_CREAT | 0666);

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigfillset(&sa.sa_mask);
    // sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    printf("SERVER STARTED!\n server id: %d\n", server_id);

    while(1){
        printf("wating for messages\n");
        if(msgrcv(server_id, &message, MSG_SIZE, -6, 0) == -1){
            perror("msgrcv");
            msgctl(server_id, IPC_RMID, NULL);
            exit(1);
            
        }
        printf("message: %s\n", message.mtext);
        react_to_communicate(&message);
        log_message(&message);
    }


    return 0;
}