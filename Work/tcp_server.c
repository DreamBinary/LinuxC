#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "pthread.h"
#include "dirent.h"
#include "string.h"
#include "msg.h"
#include <fcntl.h>


void search_server_dir(int c_fd) {
    struct dirent *dir = NULL;
    MSG send_msg = {0};
    DIR *dp = opendir("/tmp/");
    send_msg.type = MSG_TYPE_FILENAME;
    if (NULL == dp) {
        perror("open dir error");
        return;
    }
    while (1) {
        dir = readdir(dp);
        if (NULL == dir) {
            break;
        }
        if (dir->d_name[0] != '.') {
            memset(send_msg.fname, 0, sizeof(send_msg.fname));
            strcpy(send_msg.fname, dir->d_name);
            printf("filename: %s\n", send_msg.fname);
            int res = write(c_fd, &send_msg, sizeof(MSG));
            if (res < 0) {
                perror("send client error:");
                return;
            }
        }
    }
}

void search_download(int c_fd) {
    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_DOWNLOAD;
    int fd = open("/tmp/file", O_RDONLY);
    if (fd < 0) {
        perror("open file error");
        return;
    }
    while (1) {
        int res = read(fd, send_msg.buffer, sizeof(send_msg.buffer));
        if (res == 0) { // (zero indicates end of file)
            break;
        }
        write(c_fd, &send_msg, sizeof(MSG));
        memset(send_msg.buffer, 0, sizeof(send_msg.buffer));
    }
    close(fd);
    printf("finish send file\n");
}

void *thread_fun(void *arg) {
    int c_fd = *((int *) arg);
    int res;
    MSG recv_msg = {0};
    while (1) {
        res = read(c_fd, &recv_msg, sizeof(MSG));
        if (res == 0) {
            printf("client close\n");
            break;
        }
        if (recv_msg.type == MSG_TYPE_FILENAME) {
            search_server_dir(c_fd);
            memset(&recv_msg, 0, sizeof(MSG));
        } else if (recv_msg.type == MSG_TYPE_DOWNLOAD) {
            search_download(c_fd);
            memset(&recv_msg, 0, sizeof(MSG));
        }
    }
}

int main() {
    int s_fd, c_fd;
    struct sockaddr_in s_addr;
    pthread_t thread_id;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(PORT);
    bind(s_fd, (struct sockaddr *) &s_addr, sizeof(s_addr));
    listen(s_fd, 10);
    while (1) {
        printf("=============server waiting=============\n");
        c_fd = accept(s_fd, NULL, NULL);
        pthread_create(&thread_id, NULL, thread_fun, &c_fd);
    }
}