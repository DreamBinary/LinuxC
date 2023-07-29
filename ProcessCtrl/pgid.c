#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"

int main() {
    int a;
    pid_t pid = getpid();
    pid_t pgid = getpgrp();
    a = setpgid(pid, pgid);
    printf("a = %d pid = %d pgid = %d\n", a, pid, pgid);

    int b;
    pid_t pid2 = fork();
    b = setpgid(pid2, pgid);
    if (pid2 == 0) {
        printf("child progress ---> b = %d pid2 = %d pgid = %d\n", b, pid2,
               pgid);
    } else if (pid2 > 0) {
        printf("parent progress ---> b = %d pid2 = %d pgid = %d\n", b, pid2,
               pgid);
    }
    return 0;
}