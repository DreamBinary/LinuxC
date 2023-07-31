#include "stdio.h"
#include "stdlib.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "sys/types.h"
#include "unistd.h"
union semun {
    int val;               /* value for SETVAL */
    struct semid_ds* buf;  /* buffer for IPC_STAT & IPC_SET */
    unsigned short* array; /* array for GETALL & SETALL */
    struct seminfo* __buf; /* buffer for IPC_INFO */
    void* __pad;
};
int main() {
    key_t key;
    int semid, semval;
    union semun arg;
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

    while (1) {
        if ((semval = semctl(semid, 0, GETVAL, arg)) == -1) {
            perror("semctl error");
            exit(1);
        }
        if (semval > 0) {
            printf("still %d resources can be used\n", semval);
        } else {
            printf("no more resources can be used\n");
            break;
        }
        sleep(3);
    }
    exit(0);
}