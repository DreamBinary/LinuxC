#include "sys/types.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork error!\n");
    } else if (pid == 0) {
        printf("in the child process\n");
    } else {
        printf("in the parent process!\n");
    }
    exit(0);
}
