//
// Created by fivv on 2023/11/1.
//


#include "stdio.h"
#include "unistd.h"

int main() {
    int x = 5;
    if (fork()) {
        x += 30;
        printf("1:%d\n", x);
    } else
        printf("2:%d\n", x);
    printf("3:%d\n", x);
}

