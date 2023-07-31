#include "stdio.h"
#include "stdlib.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "sys/types.h"
#include "unistd.h"
#define RESOURCE 4
union semun {
    int val;               /* value for SETVAL */
    struct semid_ds* buf;  /* buffer for IPC_STAT & IPC_SET */
    unsigned short* array; /* array for GETALL & SETALL */
    struct seminfo* __buf; /* buffer for IPC_INFO */
    void* __pad;
};
int main() {
    key_t key;
    int semid;
    struct sembuf sbuf = {0, -1, IPC_NOWAIT};
    union semun arg;
    arg.val = RESOURCE;
    if ((key = ftok("/tmp/test", 'c')) == -1) {
        perror("ftok error");
        exit(1);
    }

    printf("key: %d\n", key);

    if ((semid = semget(key, 1, IPC_CREAT | 0666)) == -1) {
        perror("semget error");
        exit(1);
    }

    printf("semid: %d\n", semid);

    printf("arg.val: %d\n", arg.val);

    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl error");
        exit(1);
    }

    while (1) {
        if (semop(semid, &sbuf, 1) == -1) {
            perror("semop error");
            exit(1);
        }
        sleep(3);
    }
    semctl(semid, 0, IPC_RMID, arg);
    exit(0);
}