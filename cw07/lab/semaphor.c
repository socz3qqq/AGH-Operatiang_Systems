#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define HOME getenv("HOME") 

int main(){

    key_t key = ftok(HOME, 10);
    int sem_id;
    if(sem_id = semget(key, 3, IPC_CREAT) == -1){
        perror("semget");
    }

    if(semctl(sem_id, 1, SETALL, 1) == -1) perror("semctl");
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = 0;
    sb.sem_op = -1;
    semop(sem_id, &sb, 1);

    struct semid_ds ds;
    


    return 0;
}