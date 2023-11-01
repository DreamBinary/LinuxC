//
// Created by fivv on 2023/11/1.
//


#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main() {
    pid_t pid1 = fork();
    pid_t pid2 = fork();
    printf("pid1: %d pid2: %d\n", pid1, pid2);
    return 0;
}
