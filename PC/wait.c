#include "sys/types.h"
#include "sys/wait.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

void exit_s(int status) {
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d\n", WTERMSIG(status));
    }
}

int main() {
    pid_t pid, pid1;
    int status;
    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(0);
    } else if (pid == 0) {
        printf("the child process\n");
    }
    if (wait(&status) != pid) {
        puts("this is a parent process");
        puts("wait error");
        exit(0);
    }
    exit_s(status);

    if ((pid = fork()) < 0) {
        printf("child process error!\n");
        exit(0);
    } else if (pid == 0) {
        printf("the child process\n");
        pid1 = getpid();

        kill(pid1, 19);
    }

    if (wait(&status) != pid) {
        puts("this is a parent process");
        puts("wait error");
        exit(0);
    }
}