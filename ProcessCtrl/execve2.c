#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
extern char **environ;

int main(int argc, char *argv[]) {
    pid_t pid;
    if ((pid = fork()) < 0){
        puts("fork error");
    }
    if (pid == 0) {
        execve("hello", argv, environ);
    } else {
        puts("able to print");
    }
}