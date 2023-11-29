//
// Created by fivv on 2023/11/1.
//

#include <stdio.h>
#include <unistd.h>
#include "string.h"

int main() {
    char str[] = "abcdefghijklmnopqrstuvwxyz";
    int fd[2], child_pid;
    if (pipe(fd) < 0) {
        printf("pipe() error\n");
        return 1;
    }
    while ((child_pid = fork()) < 0);
    if (child_pid == 0) {
        printf("child process\n");
        close(fd[1]);
        char buf[100];
        int len = read(fd[0], buf, sizeof(buf));
        buf[len] = '\0';
        FILE *fp = fopen("pipo.txt", "w");
        fprintf(fp, "%s", buf);
        fclose(fp);
        fp = fopen("pipo.txt", "r");
        char buf2[100];
        fgets(buf2, sizeof(buf2), fp);
        printf("buff from pipo.txt: %s\n", buf2);
        close(fd[0]);
    } else {
        printf("parent process\n");
        close(fd[0]);
        write(fd[1], str, strlen(str));
        close(fd[1]);
    }
}