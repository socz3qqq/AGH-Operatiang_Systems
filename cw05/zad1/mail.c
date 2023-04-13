#include <stdio.h>
#define BUFF_SIZE 2048

char mail_buff[BUFF_SIZE];

void list_mails(char * mode){
    FILE * fd = popen("mail ", 'r');
    while(fgets(mail_buff, BUFF_SIZE, fd)!=NULL){
        // fwrite
    }
    pclose(fd);
}

void send_mail(char * email_addr, char * title, char * content){

}

int main(int argc, char * argv[]){

    if(argc == 2){
        list_mails(argv[1]);
    }
    else if(argc == 4){
        send_mail(argv[1], argv[2], argv[3]);
    }
    else{
        fprintf(stderr, "Invalid number of arguments...\n");
        exit(1);
    }
    

    return 0;
}