#ifndef __SHARED_MEMORY_H__
#define __SHARED_MEMORY_H__

#include "haircut_queue.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int create_shared_queue(char * filename);
Queue * attach_shared_queue(char * pathname);
int detach_shared_queue(Queue * q);
void unlink_shared_queue(char * pathname);

#endif