#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

extern char **environ;

int main(int argc, char *argv[]) {
    printf("execve process id: %ld\n", (long) getgid());
    execve("hello", argv, environ);
    puts("disable to print this msg");
}
