#include "header.h"
#include <unistd.h>

struct mq_attr mq_attr_buff;
char client_list[MAX_NO_CLIENTS][MAX_MSG_SIZE];
struct mq_attr mq_a = { .mq_maxmsg = MAX_NO_CLIENTS, .mq_msgsize = MAX_MSG_SIZE};
mqd_t client_que_desc[MAX_NO_CLIENTS], sqd;
// char msg_buff[MAX_MSG_SIZE] = "some message";
Message msg_buff;
unsigned int prio = 1;
bool all_disconnected;

void print_msg_queue_attributes(mqd_t mqdes);
mqd_t create_queue(const char * name);
void log_message();
void fill_message();
void react_to_message(int type);
void handle_stop();
void handle_init();
void handle_to_all();
void handle_to_one();
void handle_list();

int main(){
//  making a new queue
    // struct mq_attr mq_attributes;
    sqd = create_queue(SERVER_QUEUE); 

    if(sqd == -1) perror("mq_open");
    else printf("created a queue with id: %d\n", sqd);

// read queue parameters
    print_msg_queue_attributes(sqd);

    // init a connection
    while(1){
        printf("waiting for messages...\n");
        if(mq_receive(sqd, (char *) &msg_buff, MSG_BUFF_SIZE, &prio) == -1){perror("mq_receive server"); continue;};
        printf("recevied message: %s\n from: %s recipient: %s\n", msg_buff.msg, msg_buff.sender_name, msg_buff.recipient);
        log_message();
        react_to_message(prio);
    }

    // closing the queue
    mq_close(sqd);
    mq_unlink(SERVER_QUEUE);
    return 0;
}

void print_msg_queue_attributes(mqd_t mqdes){
    if(mq_getattr(mqdes, &mq_attr_buff) == -1) perror("mq_attr");
    printf(" - flags: %ld\n - max number of messages: %ld\n - max msg size:%ld\n - curr msg number: %ld\n", 
            mq_attr_buff.mq_flags, 
            mq_attr_buff.mq_maxmsg, 
            mq_attr_buff.mq_msgsize,
            mq_attr_buff.mq_curmsgs
        );
}

mqd_t create_queue(const char * name){
    struct mq_attr mq_attributes = { .mq_maxmsg = MAX_NO_CLIENTS, .mq_msgsize = MSG_BUFF_SIZE};
    return mq_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &mq_attributes);
    
}

void log_message(){
    char * my_time_string;
    time_t my_time = time(NULL);


    my_time_string= ctime(&my_time);   

    FILE * server_logs = fopen("server.log", "a");

    fprintf(server_logs, "%s, from: %s, to %s, message: %s\n\n ", 
            my_time_string, 
            msg_buff.sender_name, 
            msg_buff.recipient, 
            msg_buff.msg);

    fclose(server_logs);
}

void react_to_message(int type){
    //  get the communicate code
    switch(type){
        case INIT:
            handle_init();
            break;
        case STOP:
            handle_stop();
            break;
        case TO_ALL:
            handle_to_all();
            break;
        case TO_ONE:
            handle_to_one();
            break;
        case LIST:
            handle_list();
            break;
        default:
            break;
    }
}

void handle_init(){
    mqd_t cqd = mq_open(msg_buff.sender_name, O_WRONLY, 0666, &mq_a);
    if(cqd == -1)perror("mq_open");
    for( int i = 0; i < MAX_NO_CLIENTS; i++){
        if(strcmp(client_list[i], "") == 0){
            fill_message("Connection successfull", SERVER_QUEUE, msg_buff.sender_name);
            strcpy(client_list[i], msg_buff.sender_name);
            client_que_desc[i] = cqd;
            printf("client's fd: %d\n", client_que_desc[i]);
            if(mq_send(client_que_desc[i], (char *) &msg_buff, MSG_BUFF_SIZE, INIT) == -1) {perror("mq_send init");return;};
            return;
        }
    }
    strcpy(msg_buff.recipient, msg_buff.sender_name);
    strcpy(msg_buff.sender_name, SERVER_QUEUE);
    strcpy(msg_buff.msg, "Connection refused, too many clients...");
    if(mq_send(cqd, (char *) &msg_buff, MSG_BUFF_SIZE, ERR) == -1) {perror("mq_send init");return;};
    mq_close(cqd);
}
void handle_stop(){
    all_disconnected = true;
    for(int i = 0; i < MAX_NO_CLIENTS; i++){
        if(client_que_desc[i] > 0){
            fill_message("Disconnect from server", SERVER_QUEUE, client_list[i]);
            if(mq_send(client_que_desc[i], (char *) &msg_buff, MSG_BUFF_SIZE, STOP) == -1) {perror("mq_send stop");return;};
            if(mq_receive(sqd, (char *) &msg_buff, MSG_BUFF_SIZE, &prio) == -1) {
                perror("mq_receive stop"); 
                all_disconnected = false;
                continue;
            }
            if(mq_close(client_que_desc[i]) == -1)perror("mq_close");
            client_que_desc[i] = 0;
            strcpy(client_list[i], "");
        }
    }
    if(all_disconnected){
        printf("Disconnected all users, closing...\n");
        exit(0);
    }else{
        fprintf(stderr, "Failed to disconnect some clients, trying again...\n");
        handle_stop();
    }
    
};
void handle_to_all(){
    for(int i = 0 ; i < MAX_NO_CLIENTS; i++){
        if(client_que_desc[i] > 0){
            strcpy(msg_buff.recipient, client_list[i]);
            if(mq_send(client_que_desc[i], (char *) &msg_buff, MSG_BUFF_SIZE,TO_ALL) == -1)perror("mq_send to all");
        }
    }
};
void handle_to_one(){
    for(int i = 0 ; i < MAX_NO_CLIENTS; i++){
        if(strcmp(msg_buff.sender_name, client_list[i])==0){
            if(mq_send(client_que_desc[i], (char *) &msg_buff, MSG_BUFF_SIZE,TO_ONE) == -1)perror("mq_send to all");
        }
    }
};
void handle_list(){
    mqd_t recipient;
    strcpy(msg_buff.msg, "");
    for(int i = 0; i < MAX_NO_CLIENTS; i++){
        printf("%d \n", client_que_desc[i]);
        if(client_que_desc[i] > 0){
            strcat(msg_buff.msg, client_list[i]);
        }
        if(strcmp(msg_buff.sender_name, client_list[i]) == 0){
            recipient = client_que_desc[i];
            printf("found one! %d\n", recipient);
        }
    }
    strcpy(msg_buff.recipient, msg_buff.sender_name);
    strcpy(msg_buff.sender_name, SERVER_QUEUE);
    if(mq_send(recipient, (char *) &msg_buff, MSG_BUFF_SIZE, LIST) == -1)perror("mq_send list");
    // if(mq_receive(sqd, (char *) &msg_buff, MSG_BUFF_SIZE, &prio) == -1)perror("mq_receive list");     
};

void fill_message(char * message, char * sender, char * client){
    strcpy(msg_buff.msg, message);
    strcpy(msg_buff.recipient, client);
    strcpy(msg_buff.sender_name, sender);
}