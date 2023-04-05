#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFF_SIZE 1024

typedef enum { false, true } boolean;

boolean directoryExists(char * path);
