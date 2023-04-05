#ifndef _REC_WC_BYTES_H_
#define _REC_WC_BYTES_H_

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ftw.h>
#include <errno.h>
#include <string.h>

#define CWD_BUFF_SIZE 256
#define MAX_DIR 64

typedef enum{false, true} bool;
int process_file(const char * fpath, const struct stat * sb, int typeflag);
bool path_valid(char * path);

#endif