#ifndef __HEADERS_H__
#define __HEADERS_H__

#define CLIENT "./client"
#define HAIRDRESSER "./hairdresser"
#define SHARED_MEMORY getenv("HOME")
#define MUTEX_PATH getenv("HOME")
#define TIMEOUT 10
#define CLIENTS_NUMBER 20
#define MAX_QUEUE_SIZE 1024
#define WAITING_ROOM 6

#define SEM_HAIRDRESSER_NAME "0"
#define SEM_WAIT_NAME "1"
#define SEM_QUEUE_MUTEX_NAME "2"
#define SEM_HAIRCUTS_NAME "3"
#define SEM_CHAIRS_NAME "4"

char error_message[128];

int sem_hairdresser, sem_wait, sem_queue_mutex, sem_haircuts, sem_chairs;

#endif