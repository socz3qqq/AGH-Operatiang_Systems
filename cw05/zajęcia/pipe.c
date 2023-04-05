#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main (){
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) { // dziecko
        close(fd[1]);
        char child_buff[20] = "ok";
        int i = 0;
        while(read(fd[0], &child_buff[i], 1) != EOF){
            i++;
        } // - odczyt danych z potoku
        printf("%s \n", child_buff);
    } 
    else { // rodzic
        close(fd[0]);
        char * stri = "hello world";
        write(fd[1], stri, 10); // - zapis danych do potoku
    }
    return 0;
}