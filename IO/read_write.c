#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"

int main() {
    char* path = "tmp.txt";
    int fd;
    char buf[40], buf2[] = "Hello, world!";
    int n, i;
    if ((fd = open(path, O_RDWR)) < 0) {
        printf("open error\n");
        return 1;
    } else {
        printf("open success\n");
    }

    if ((n = read(fd, buf, 40)) < 0) {
        printf("read error\n");
        return 1;
    } else {
        printf("read success\n");
        printf("read %d bytes\n", n);
        printf("buf: %s\n", buf);
    }

    if (i = lseek(fd, 6, SEEK_SET) < 0) {
        printf("lseek error\n");
        return 1;
    } else {
        printf("lseek success\n");
    }
    if ((n = write(fd, buf2, 13)) < 0) {
        printf("write error\n");
        return 1;
    } else {
        printf("write success\n");
        printf("write %d bytes\n", n);
    }
    close(fd);

    if ((fd = open(path, O_RDWR)) < 0) {
        printf("open error\n");
        return 1;
    } else {
        printf("open success\n");
    }

    if ((n = read(fd, buf, 40)) < 0) {
        printf("read error\n");
        return 1;
    } else {
        printf("read success\n");
        printf("read %d bytes\n", n);
        printf("buf: %s\n", buf);
    }
    close(fd);
    return 0;
}