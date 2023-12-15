#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/wait.h"

#define MAXSIZE 100

int main() {
    int fd[2], pid, line;
    char msg[MAXSIZE];
    if (pipe(fd) == -1) {
        perror("create pipe failed");
        return 1;
    } else if ((pid = fork()) < 0) {
        perror("not create a new process!");
        return 1;
    } else if (pid == 0) {
        close(fd[0]);
        printf("child process send msg!\n");
        write(fd[1], "Hello World!", 13);
    } else {
        close(fd[1]);
        printf("parent process receive msg is: \n");
        line = read(fd[0], msg, MAXSIZE);
        write(STDOUT_FILENO, msg, line);
        printf("\n");
        wait(NULL);
        _exit(0);
    }
    return 0;
}