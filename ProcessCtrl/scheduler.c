#include "sched.h"
#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"

int main() {
    pid_t pid_a = fork();
    if (pid_a == 0) {
        printf("child progress ---> pid_a = %d\n", pid_a);
        return 0;
    } else if (pid_a > 0) {
        printf("parent progress ---> pid_a = %d\n", pid_a);
    } else {
        printf("fork error!\n");
    }

    pid_t pid_b = fork();
    if (pid_b == 0) {
        printf("child progress ---> pid_b = %d\n", pid_b);
        return 0;
    } else if (pid_b > 0) {
        printf("parent progress ---> pid_b = %d\n", pid_b);
    } else {
        printf("fork error!\n");
    }

    // sched_setscheduler(pid_a, SCHED_OTHER, 0);
    // sched_setscheduler(pid_b, SCHED_FIFO, 0);

    // printf("pid_a scheduler policy: %d\n", sched_getscheduler(pid_a));
    // printf("pid_b scheduler policy: %d\n", sched_getscheduler(pid_b));

    return 0;
}