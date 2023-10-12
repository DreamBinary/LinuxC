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
#include <fcntl.h>
#include "share.h"

char server_path[50] = "/root/LinuxC/Work/server_download/";
char server_upload_path[50] = "/root/LinuxC/Work/server_upload/";

void send_server_filename(int c_fd) {
    struct dirent *dir = NULL;
    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_FILENAME;
    DIR *dp = opendir(server_path);
    if (NULL == dp) {
        perror("open dir error");
        return;
    }
    char filenames[1024] = "";
    while (1) {
        dir = readdir(dp);
        if (NULL == dir) {
            break;
        }
        if (dir->d_name[0] != '.') {
            strcat(filenames, dir->d_name);
            strcat(filenames, "\n");
        }
    }
    strcpy(send_msg.buffer, filenames);
    int res = write(c_fd, &send_msg, sizeof(MSG));
    if (res < 0) {
        perror("send client error:");
    }
    printf("finish send filename\n");
}

void send_download_file(int c_fd, char *filename) {
    printf("%s\n", filename);
    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_DOWNLOAD;
    char filepath[100] = "";
    strcpy(filepath, server_path);
    strcat(filepath, filename);
    send_file(c_fd, &send_msg, filepath);
    printf("finish send download file\n");
}

void save_upload_file(MSG recv_msg, char *upload_filename) {
    char filepath[100] = "";
    strcpy(filepath, server_upload_path);
    strcat(filepath, upload_filename);
    write_file(filepath, recv_msg.buffer, sizeof(recv_msg.buffer));
    printf("finish save upload file\n");
}

void *recv_thread(void *arg) {
    int c_fd = *((int *) arg);
    char upload_filename[50];
    MSG recv_msg = {0};
    while (1) {
        int res = read(c_fd, &recv_msg, sizeof(MSG));
        if (res == 0) {
            printf("client close\n");
            break;
        }
        if (recv_msg.type == MSG_TYPE_FILENAME) {
            send_server_filename(c_fd);
        } else if (recv_msg.type == MSG_TYPE_DOWNLOAD) {
            send_download_file(c_fd, recv_msg.fname);
        } else if (recv_msg.type == MSG_TYPE_UPLOAD_FLAG) {
            strcpy(upload_filename, recv_msg.fname);
            create_dir("Work/server_upload");
        } else if (recv_msg.type == MSG_TYPE_UPLOAD_DATA) {
            save_upload_file(recv_msg, upload_filename);
        }
        memset(&recv_msg, 0, sizeof(MSG));
    }
}

int main() {
    int s_fd, c_fd;
    struct sockaddr_in s_addr, c_addr;
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
        int len = sizeof(c_addr);
        c_fd = accept(s_fd, (struct sockaddr *) &c_addr, &len);
        printf("========client addr:%s connected========\n", inet_ntoa(c_addr.sin_addr));
        pthread_create(&thread_id, NULL, recv_thread, &c_fd);
    }
}