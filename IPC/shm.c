#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/ipc.h"
#include "sys/shm.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"

int main() {
    int shmid;
    int proj_id;
    key_t key;
    int size;
    char *addr;
    pid_t pid;
    key = IPC_PRIVATE;
    shmid =
            shmget(key, 1024,
                   IPC_CREAT | 0666);  // IPC_CREAT | 0666 --> valid simutaniously

    if (shmid == -1) {
        perror("shmget error");
        exit(1);
    } else {
        printf("shmget success\n");
    }
    addr = (char *) shmat(shmid, NULL, 0);
    if (addr == (char *) -1) {
        perror("shmat error");
        exit(1);
    } else {
        printf("shmat success\n");
    }
    printf("share memory addr is: %p\n", addr);
    strcpy(addr, "Hello World");
    pid = fork();
    if (pid == -1) {
        perror("fork error");
    } else if (pid == 0) {
        printf("child process string is %s\n", addr);
        _exit(0);
    } else {
        wait(NULL);
        printf("parent progress string is %s\n", addr);
        if (shmdt(addr) == -1) {
            perror("release failed!");
            return 1;
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("failed!");
            return 1;
        }
    }
    return 0;
}