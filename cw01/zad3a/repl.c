#include <regex.h>
#include <sys/times.h>
#include <time.h>
#include "wordcount.h"
#define CLOCK_REALTIME 0


int main(){ 
    char cmd[BUFF_SIZE];

    string_table  * wc_table = NULL;

    regex_t regINIT, regCOUNT, regSHOW, regDELETE, regDESTROY; 

    int cmd_arg = -1;

    struct tms tms_start, tms_end;
    struct timespec tspec_start, tspec_end;

    regcomp(&regINIT, "init [0-9][0-9]*", 0);
    regcomp(&regCOUNT, "count .*", 0);
    regcomp(&regSHOW, "show [0-9][0-9]*", 0);
    regcomp(&regDELETE, "delete [0-9][0-9]*", 0);
    regcomp(&regDESTROY, "destroy", 0);

    printf("[REPL] $ ");

    while( fgets(cmd, BUFF_SIZE, stdin) != NULL ){

        if ( regexec(&regINIT, cmd, 0, NULL, 0) == 0){
            if(wc_table != NULL){
                fprintf(stderr, "Table is already allocated\n");
            }
            else{
                sscanf(cmd, "init %d", &cmd_arg);
                if( cmd_arg > 0){
                    clock_gettime(CLOCK_REALTIME, &tspec_start);
                    times(&tms_start);
                    wc_table = init_table(cmd_arg);
                    clock_gettime(CLOCK_REALTIME, &tspec_end);
                    times(&tms_end);
                }
                else{
                    fprintf(stderr, "INVALID ARGUMENT\n");
                }
            }
        }
        else{
            if(wc_table == NULL){
                fprintf(stderr, "You need to init a table before using it :)\n");
            }
            else if( regexec(&regCOUNT, cmd, 0, NULL, 0) == 0 ){
                if(wc_table != NULL){
                    char path[1025] = "";
                    sscanf(cmd, "count %s", path);

                    char test_command[BUFF_SIZE];
                    snprintf(test_command, BUFF_SIZE, "ls '%s' 1> /dev/null 2> /dev/null", path);

                    if( system(test_command) == 0){
                        clock_gettime(CLOCK_REALTIME, &tspec_start);
                        times(&tms_start);
                        add_element(wc_table, path);
                        clock_gettime(CLOCK_REALTIME, &tspec_end);
                        times(&tms_end);
                    }
                    else{
                        fprintf(stderr, "PATH: %s IS INVALID\n", path);
                    }
                }
                else{
                    fprintf(stderr, "You need to init a table before using it :)\n");
                }
            }
            else if( regexec(&regSHOW, cmd, 0, NULL, 0) == 0){
                sscanf(cmd, "show %d", &cmd_arg);
                clock_gettime(CLOCK_REALTIME, &tspec_start);
                times(&tms_start);
                char * result = get_by_index(wc_table, cmd_arg);
                clock_gettime(CLOCK_REALTIME, &tspec_end);
                times(&tms_end);
                printf("%s\n", result);
            }
            else if( regexec(&regDELETE, cmd, 0, NULL, 0) == 0){
                sscanf(cmd, "delete %d", &cmd_arg);
                clock_gettime(CLOCK_REALTIME, &tspec_start);
                times(&tms_start);
                delete_by_index(wc_table, cmd_arg);
                clock_gettime(CLOCK_REALTIME, &tspec_end);
                times(&tms_end);
            }
            else if( regexec(&regDESTROY, cmd, 0, NULL, 0) == 0){
                clock_gettime(CLOCK_REALTIME, &tspec_start);
                times(&tms_start);
                free_struct(wc_table);
                clock_gettime(CLOCK_REALTIME, &tspec_end);
                times(&tms_end);
                wc_table = NULL;
                printf("Succesfully deleted table, now you can init another\n");
            }
            else{
                fprintf(stderr, "INVALID COMMAND\n");
            }   
        }
        fflush(NULL);
        printf("EXEC TIMES:\n");
        printf("REAL: %ld nanoseconds \n", tspec_end.tv_nsec - tspec_start.tv_nsec);
        printf("USER: %ld seconds \n", tms_end.tms_cutime - tms_start.tms_cutime);
        printf("SYSTEM: %ld seconds \n", tms_end.tms_cstime - tms_start.tms_cstime);       
        printf("[REPL] $ ");
    }
}