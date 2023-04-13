#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


#define PATH_FIFO "/tmp/integral_pipe"

long double f(double x){
    return 4.0/(x*x + 1.0);
}
    
int main(int argc, char * argv[]){
    if(argc != 4){
        fprintf(stderr, "invalid number of arguments\n");
    }
    int named_pipe = open(PATH_FIFO, O_WRONLY);
    double start = strtod(argv[1], NULL);
    double end = strtod(argv[2], NULL);
    double interval = strtod(argv[3], NULL);

    double output = 0.0;
    for(double x = start; x < end; x += interval){
        output += f(x)*interval;
    }
    if( named_pipe < 0 ) {
        perror("Error in function open");
        return 1;
    }
    write(named_pipe, &output, sizeof(double));
    close(named_pipe);
    return 0;
}