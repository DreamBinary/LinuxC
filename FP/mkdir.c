#include "sys/stat.h"
#include "sys/types.h"
#include "stdio.h"

int main() {
    char *path = "test";
    int status = mkdir(path, 0777);
    if (status == -1) {
        perror("mkdir() error");
        return -1;
    } else {
        printf("Directory created\n");
        return 0;
    }
}