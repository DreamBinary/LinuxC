/*****************************
// Created by CXQ on 2023/9/13.
*****************************/

#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "pthread.h"
#include "dirent.h"
#include "string.h"
#include "msg.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


void send_server_filename(int c_fd) {
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

void send_download_file(int c_fd) {
    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_DOWNLOAD;
    int fd = open("Work/server_download/server_file", O_RDONLY);
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

char upload_filename[50];

void create_upload_file() {
    if (access("Work/server_upload", F_OK) == -1) {
//                printf("download not exist\n");
        int res = mkdir("Work/server_upload", 0777);
        if (res == -1) {
            perror("mkdir() error");
        }
    }
}

void save_upload_file(MSG recv_msg) {
    char filepath[100] = "Work/server_upload/";
    strcat(filepath, upload_filename);
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd < 0) {
        perror("open file error");
    } else {
        write(fd, recv_msg.buffer, sizeof(recv_msg.buffer));
        memset(recv_msg.buffer, 0, sizeof(recv_msg.buffer));
        close(fd);
    }
}

void *recv_thread(void *arg) {
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
            send_server_filename(c_fd);
            memset(&recv_msg, 0, sizeof(MSG));
        } else if (recv_msg.type == MSG_TYPE_DOWNLOAD) {
            send_download_file(c_fd);
            memset(&recv_msg, 0, sizeof(MSG));
        } else if (recv_msg.type == MSG_TYPE_UPLOAD_FLAG) {
            strcpy(upload_filename, recv_msg.fname);
            create_upload_file();
            memset(&recv_msg, 0, sizeof(MSG));
        } else if (recv_msg.type == MSG_TYPE_UPLOAD_DATA) {
            save_upload_file(recv_msg);
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
        struct sockaddr_in c_addr;
        int len = sizeof(c_addr);
        c_fd = accept(s_fd, (struct sockaddr *) &c_addr, &len);
        printf("========client addr:%s connected========\n", inet_ntoa(c_addr.sin_addr));
        pthread_create(&thread_id, NULL, recv_thread, &c_fd);
    }
}