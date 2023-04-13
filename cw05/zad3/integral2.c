#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

#define PATH_FIFO "/tmp/integral_pipe"
#define BUFF_SIZE 128

double sol_buff;
char arg_start[BUFF_SIZE];
char arg_end[BUFF_SIZE];

void print_time(struct timespec start, struct timespec end){
    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    printf("Processing time: %.4lf\n\n", time_taken);
}


int main(int argc, char * argv[]){
    if ( argc != 3){
        fprintf(stderr, "Invalid arguments, please provide interval and process count\n");
        exit(1);
    }
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    int program_count = atoi(argv[2]);
    pid_t pid;
    if (mkfifo(PATH_FIFO, S_IFIFO | S_IRWXU) <0){
        perror("mkfifo: ");
    }
    for(int i = 0; i < program_count; i++){
        if( (pid = fork()) < 0){
            perror("Fork error");
        }
        else if(pid == 0){
            sprintf(arg_start, "%lf",  i*(1.0/program_count));
            sprintf(arg_end, "%lf", (i+1.0)*(1.0/program_count));
            execl("./sub_integral", "sub_integral", arg_start, arg_end, argv[1], NULL);
            exit(1);
        }
    }
    int named_pipe = open(PATH_FIFO, O_RDONLY);
    double solution = 0.0;
    for(int i = 0; i<program_count; i++){
        if(read(named_pipe, &sol_buff, sizeof(double)) < 0){
            perror("Error in read");
        }
        else{
            solution += sol_buff;
        }

    }
    close(named_pipe);
    unlink(PATH_FIFO);
    printf("Interval: %.10f, Processess: %d\n", strtod(argv[1], NULL), atoi(argv[2]));
    printf("Solution: %lf\n", solution);
    clock_gettime(CLOCK_REALTIME, &end_time);
    print_time(start_time, end_time);
    return 0;
}