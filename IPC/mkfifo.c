#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"
#define FIFO "/tmp/myfifo"

int main() {
    int fd;
    int pid;
    char r_msg[BUFSIZ];
    if ((pid = mkfifo(FIFO, 0777)) == -1) {
        perror("mkfifo error");
        exit(1);
    } else {
        printf("mkfifo success\n");
    }

    if ((fd = open(FIFO, O_RDWR)) == -1) {
        perror("open error");
        exit(1);
    } else {
        printf("open success\n");
    }

    if (write(fd, "Hello World", 12) == -1) {
        perror("write error");
        exit(1);
    } else {
        printf("write success\n");
    }

    if (read(fd, r_msg, BUFSIZ) == -1) {
        perror("read error");
        exit(1);
    } else {
        printf("read success\n");
        printf("read msg is: %s\n", r_msg);
    }

    return 0;
}
