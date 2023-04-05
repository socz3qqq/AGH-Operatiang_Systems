#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#define CLOCK_REALTIME 0

typedef enum{false, true} bool;

bool is_ASCII(char * s){
    char ascii;
    if( sizeof(*s) == sizeof(char)){
        ascii = s[0];
        if ((int)ascii> 127 || (int)ascii < 32){
            fprintf(stderr, "Argument: \"%s\" is invalid, please enter a (visible) character with ASCII code between 32 and 127 \n", s);
            return false;
        }
    }
    else {
        fprintf(stderr, "Argument: \"%s\" is invalid, please enter a valid ASCII character \n", s);
        return false;
    }
    return true;
}


int main(int argc, char * argv[]){

    struct timespec timespec_start, timespec_end;
    struct tms start_time, end_time;
    if( times(&start_time) < 0)
        fprintf(stderr, "Failed to get time info\n");
    if( clock_gettime(CLOCK_REALTIME, &timespec_start) < 0)
        fprintf(stderr, "Failed to get clock realtime info.\n");

    // declaring some variables
    char to_replace, replace_val;
    char * input_file = argv[3], * output_file = argv[4];
    FILE * fd_in, * fd_out;
    if ( argc != 5){ 
        fprintf(stderr, "Please provide 4 arguments \n 1 - ASCII char to replace \n 2 - ASCII char to replace with \n 3 - input file path \n 4 - output file path\n");
        return 1;
    }

    //checking the input
    if(is_ASCII(argv[1])){
        to_replace = (char)argv[1][0];
    }
    else
        return 1;

    if(is_ASCII(argv[2]))
        replace_val = (char)argv[2][0];
    else
        return 1;
    // opening files
    if ( (fd_in = fopen(input_file, "r"))  == NULL){
        fprintf(stderr, "Failed to open file: %s\n", input_file);
        return 3;
    }
    if( (fd_out = fopen(output_file, "w")) == NULL){
        fprintf(stderr, "Failed to open file: %s\n", output_file);
        return 3;
    }
    fseek(fd_in, 0, SEEK_SET);
    char buff;
    while(fread(&buff, sizeof(char), 1, fd_in) == 1){
        if ( buff ==  to_replace){
            if( fwrite(&replace_val, sizeof(char), 1, fd_out) != 1){
                fprintf(stderr, "Failed to write into a file: %s \n", output_file);
                return 5;
            }  
        } 
        else
            if(fwrite(&buff, sizeof(char), 1, fd_out) != 1){
                fprintf(stderr, "Failed to write into a file: %s \n", output_file);
                return 5;
            }
    }
    fclose(fd_in);
    fclose(fd_out);

    if( times(&end_time) < 0)
        fprintf(stderr, "Failed to get time info\n");
    if( clock_gettime(CLOCK_REALTIME, &timespec_end) < 0)
        fprintf(stderr, "Failed to get clock realtime info.\n");

    int time_stats = open("pomiar_zad_1.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    write(time_stats, "replace_lib: \n", 14);
    char output_buff[128] = "Total time: ";
    if ( write(time_stats, output_buff, 12) < 0)
        fprintf(stderr, "Failed to write to a file\n");
    long seconds = timespec_end.tv_sec - timespec_start.tv_sec;
    long nanoseconds = timespec_end.tv_nsec - timespec_start.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    int n = snprintf(output_buff, 128, "%3f", elapsed);
    write(time_stats, output_buff, n);
    write(time_stats, " sec\n", 5);

    strcpy(output_buff, "System time: ");
    write(time_stats, output_buff, 13);
    n = snprintf(output_buff, 128, "%.6f", (float)(end_time.tms_stime - start_time.tms_stime)/sysconf(_SC_CLK_TCK)  
                                        + (float)(end_time.tms_cstime - start_time.tms_cstime)/sysconf(_SC_CLK_TCK));
    write(time_stats, output_buff, n);
    write(time_stats, " sec\n", 5);

    strcpy(output_buff,"User time: ");
    write(time_stats, output_buff, 11);
    n = snprintf(output_buff, 128, "%.6f", (float)(end_time.tms_utime - start_time.tms_utime)/sysconf(_SC_CLK_TCK) 
                                        + (float)(end_time.tms_cutime - start_time.tms_cutime)/sysconf(_SC_CLK_TCK));
    write(time_stats, output_buff, n);
    write(time_stats, " sec\n\n", 6);
    return 0;
}