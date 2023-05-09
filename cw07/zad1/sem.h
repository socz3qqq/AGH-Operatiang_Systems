#ifndef __SEM_H__
#define __SEM_H__

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

int create_sem(const char * filename, int val);
int open_sem(const char * filename);
void unlink_sem(const char * filename);
int acquire(int semid, int flags);
void release(int semid, int flags);

#endif
