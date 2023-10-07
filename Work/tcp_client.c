/*****************************
// Created by CXQ on 2023/9/13.
*****************************/

#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "msg.h"
#include "pthread.h"
#include <sys/stat.h>
#include <fcntl.h>

void net_desk_ui() {
    printf("=============TCP文件传输客户端=============\n");
    printf("------------------功能菜单------------------\n");
    printf("\t\t1.查询文件\n");
    printf("\t\t2.下载文件\n");
    printf("\t\t3.上传文件\n");
    printf("\t\t0.退出系统\n");
    printf("------------------------------------------------\n");
    printf("请选择你要执行的操作：\n");
}

void *recv_thread(void *arg) {
    int s_fd = *((int *) arg);
    MSG recv_msg = {0};
    while (1) {
        read(s_fd, &recv_msg, sizeof(MSG));
        if (recv_msg.type == MSG_TYPE_FILENAME) {
            printf("server path filename = %s\n", recv_msg.fname);
            memset(&recv_msg, 0, sizeof(MSG));
        } else if (recv_msg.type == MSG_TYPE_DOWNLOAD) {
            if (access("Work/client_download", F_OK) == -1) {
                int res = mkdir("Work/client_download", 0777);
                if (res == -1) {
                    perror("mkdir() error");
                }
            }
            int fd = open("Work/client_download/file", O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd < 0) {
                perror("open file error");
            } else {
                write(fd, recv_msg.buffer, sizeof(recv_msg.buffer));
                memset(recv_msg.buffer, 0, sizeof(recv_msg.buffer));
                close(fd);
            }
        }
    }
}

char upload_filename[50];

void *upload(void *arg) {
    int s_fd = *((int *) arg);
    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_UPLOAD_DATA;
    char filepath[100] = "Work/client_upload/";
    strcat(filepath, upload_filename);
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("open file error");
    }
    while (1) {
        int res = read(fd, send_msg.buffer, sizeof(send_msg.buffer));
        if (res == 0) { // (zero indicates end of file)
            break;
        }
        write(s_fd, &send_msg, sizeof(MSG));
        memset(send_msg.buffer, 0, sizeof(send_msg.buffer));
    }
    close(fd);
}

int main() {
    int s_fd;
    int res;
    char choice;
    struct sockaddr_in address;
    pthread_t recv_thread_id;
    pthread_t send_thread_id;
    MSG send_msg = {0};
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    int news_fd = connect(s_fd, (struct sockaddr *) &address, sizeof(address));
    if (news_fd == -1) {
        printf("connect error\n");
        return 1;
    }
    pthread_create(&recv_thread_id, NULL, recv_thread, &s_fd);
    net_desk_ui();
    while (1) {
        choice = getchar();
        switch (choice) {
            case '1':
                send_msg.type = MSG_TYPE_FILENAME;
                res = write(s_fd, &send_msg, sizeof(MSG));
                if (res < 0) {
                    perror("send msg error:");
                }
                memset(&send_msg, 0, sizeof(MSG));
                break;
            case '2':
                send_msg.type = MSG_TYPE_DOWNLOAD;
                res = write(s_fd, &send_msg, sizeof(MSG));
                if (res < 0) {
                    perror("send msg error:");
                }
                memset(&send_msg, 0, sizeof(MSG));
                break;
            case '3':
                send_msg.type = MSG_TYPE_UPLOAD_FLAG;
                printf("input filename:");
                scanf("%s", upload_filename);
                strcpy(send_msg.fname, upload_filename);
                res = write(s_fd, &send_msg, sizeof(MSG));
                if (res < 0) {
                    perror("upload error:");
                }
                memset(&send_msg, 0, sizeof(MSG));
                pthread_create(&send_thread_id, NULL, upload, &s_fd);
                break;
            case '0':
                exit(0);
            default:
//                printf("input error\n");
                break;
        }
        net_desk_ui();
    }
//
//    close(s_fd);
//    return 0;
}