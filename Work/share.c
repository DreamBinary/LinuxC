/*****************************
// Created by CXQ on 2023/9/13.
*****************************/

#include "share.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/stat.h"
#include "string.h"
#include <fcntl.h>
#include "msg.h"
#include "share.h"

void create_dir(char *path) {
    if (access(path, F_OK) == -1) {
        int res = mkdir(path, 0777);
        if (res == -1) {
            perror("mkdir error");
        }
    }
}

void write_file(char *filepath, char *buffer, int size) {
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd < 0) {
        perror("open file error");
    } else {
        write(fd, buffer, size);
        memset(buffer, 0, size);
        close(fd);
    }
}

void send_file(int ffd, MSG *send_msg, char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("open file error");
    }
    while (1) {
        int res = read(fd, send_msg->buffer, sizeof(send_msg->buffer));
        if (res == 0) { // (zero indicates end of file)
            break;
        }
        write(ffd, send_msg, sizeof(MSG));
        memset(send_msg->buffer, 0, sizeof(send_msg->buffer));
    }
    close(fd);
}