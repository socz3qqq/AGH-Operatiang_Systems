#include "header.h"
#include <fcntl.h>
#include <sys/stat.h>

// char msg_buff[MAX_MSG_SIZE];
char client_name[NAME_SIZE];
Message msg_buff;
struct mq_attr mq_attributes = { .mq_maxmsg = MAX_NO_CLIENTS, .mq_msgsize = (long) MSG_BUFF_SIZE};

mqd_t create_queue(const char * name);
mqd_t open_server_queue(const char * name);
void asign_name();
char rand_char();
void print_msg_queue_attributes(mqd_t mqdes);

int main(){
// assigning a name to the client
    asign_name();
    bool connected = false;
    size_t len = 0;
    size_t read;
    unsigned int prio;
    char * cmd = NULL;
    char * command = NULL;
// opening queues
    mqd_t sqdes = open_server_queue(SERVER_QUEUE);
    mqd_t cqdes = create_queue(client_name);

    while(1){
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
            strcpy(msg_buff.msg, client_name);
            strncpy(msg_buff.sender_name, client_name, NAME_SIZE);
            strcpy(msg_buff.recipient, SERVER_QUEUE);
            if(mq_send(sqdes, (char *) &msg_buff, MSG_BUFF_SIZE, INIT) == -1) { perror("mq_send_init"); continue;};
            if(mq_receive(cqdes, (char *) &msg_buff, MSG_BUFF_SIZE, &prio) == -1){perror("mq_receive_init"); continue;}
            printf("Message from %s: %s\n", msg_buff.sender_name, msg_buff.msg);
            if(prio) connected = true;
        }
        else if(strcmp(command, "LIST") == 0){
            //  handle listid;
            strcpy(msg_buff.msg, "Could you please give me th list of all users?");
            strncpy(msg_buff.sender_name, client_name, NAME_SIZE);
            strcpy(msg_buff.recipient, SERVER_QUEUE);
            if(mq_send(sqdes, (char *) &msg_buff, MSG_BUFF_SIZE, LIST) == -1) { perror("mq_send_init"); continue;};
            if(mq_receive(cqdes, (char *) &msg_buff, MSG_BUFF_SIZE, &prio) == -1){perror("mq_receive_init"); continue;}
            printf("Message from %s: %s\n", msg_buff.sender_name, msg_buff.msg);
        }
        else if(strcmp(command, "2ALL") == 0){
            // handle sending messages to all
            command = strtok(NULL, " ");

            strcpy(msg_buff.msg, command);
            strncpy(msg_buff.sender_name, client_name, NAME_SIZE);
            strcpy(msg_buff.recipient, SERVER_QUEUE);
            if(mq_send(sqdes, (char *) &msg_buff, MSG_BUFF_SIZE, TO_ALL) == -1) { perror("mq_send_init"); continue;};
        }
        else if(strcmp(command, "2ONE") == 0){
            // handle senging a message
            command = strtok(NULL, " ");
            strcpy(msg_buff.recipient, command);
            command = strtok(NULL, " ");
            strcpy(msg_buff.msg, command);
            strncpy(msg_buff.sender_name, client_name, NAME_SIZE);
            if(mq_send(sqdes, (char *) &msg_buff, MSG_BUFF_SIZE, TO_ALL) == -1) { perror("mq_send_init"); continue;};
        }
        else{
            fprintf(stderr, "Invalid command!\n");
            continue;
        }
    }

    free(command);
    
    return 0;
}

mqd_t open_server_queue(const char * name){
    // struct mq_attr mq_attributes = { .mq_maxmsg = MAX_NO_CLIENTS, .mq_msgsize = MSG_BUFF_SIZE + 1};
    return mq_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &mq_attributes);
}
mqd_t create_queue(const char * name){
    // struct mq_attr mq_attributes = { .mq_maxmsg = MAX_NO_CLIENTS, .mq_msgsize = MSG_BUFF_SIZE + 1};
    return mq_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &mq_attributes);
    
}
void asign_name(){
    client_name[0] = '/';
    for( int i = 1; i < NAME_SIZE - 1; i++){
        client_name[i] = rand_char();
    }
}
char rand_char(){
    const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t  char_count= sizeof(chars) - 1;
    srand(time(NULL));
    char random_char = chars[rand() % char_count];
    return random_char;
}

void print_msg_queue_attributes(mqd_t mqdes){
    if(mq_getattr(mqdes, &mq_attributes) == -1) perror("mq_attr");
    printf(" - flags: %ld\n - max number of messages: %ld\n - max msg size:%ld\n - curr msg number: %ld\n", 
            mq_attributes.mq_flags, 
            mq_attributes.mq_maxmsg, 
            mq_attributes.mq_msgsize,
            mq_attributes.mq_curmsgs
        );
}