#include "child_proc.h"

int main(int argc, char * argv[]){
    // check the data
    int child_pid = getpid();
    if ( argc != 2){
        fprintf(stderr, "Invalid number of arguments, please enter only one argument - amount of child processes\n");
        return 2;
    }
    int count = 0;
    if ((count = atoi(argv[1])) == 0){
        fprintf(stderr, "Argument should be total number\n");
        return 2;
    }

    // create child proccesses
    for (size_t i = 0; i < count; i++)
    {
        if( child_pid > 0){
            child_pid = fork();
            if(child_pid == 0){
                printf("Forked PID: %d, Parent PID: %d\n", getpid(), getppid());
            }
        }
    }
    // wait for all created children to end
    for (size_t i = 0; i < count; i++)
    {
        wait(NULL);
    }
    // print children number
    if(child_pid > 0){
        printf("argv[1]: %d\n", count);
    }
    // success!
    return 0;
}