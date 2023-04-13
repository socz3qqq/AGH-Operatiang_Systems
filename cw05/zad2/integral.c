#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#define SEC_TO_NANO 1000000000

double sol_buff;

double f(double x){
    return 4.0/(x*x + 1.0);
}

double integral(double start, double end, double interval){
    double output = 0.0;
    for(double x = start; x < end; x += interval){
        output += f(x)*interval;
    }
    return output;
}

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
    double interval = strtod(argv[1], NULL);
    int process_count = atoi(argv[2]);
    pid_t pid;
    int fd[2];
    pipe(fd); 
    for(int i = 0; i < process_count; i++){
        double partial_output;

        pid = fork();
        if(pid == 0){
            close(fd[0]);
            partial_output = integral(i*(1.0/process_count), (i+1.0)*(1.0/process_count), interval);
            write(fd[1], &partial_output, sizeof(double));
            exit(0);
        }
    }
    while(wait(NULL) > 0); //waiting for all child processes to end

    // calculating the result
    double solution = 0.0;
    for(int i = 0; i<process_count; i++){
        read(fd[0], &sol_buff, sizeof(double));
        solution += sol_buff;
    }
    printf("Interval: %.10f, Processess: %d\n", strtod(argv[1], NULL), atoi(argv[2]));
    printf("Solution: %lf\n", solution);
    clock_gettime(CLOCK_REALTIME, &end_time);
    print_time(start_time, end_time);
    return 0;
}