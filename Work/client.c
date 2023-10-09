/*****************************
// Created by CXQ on 2023/9/13.
*****************************/

#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h"
#include "share.h"
#include "msg.h"

char filepath[50];
char filename[50];

void net_desk_ui() {
    printf("===============TCP文件传输客户端===============\n");
    printf("------------------功能菜单------------------\n");
    printf("\t\t1.查询文件\n");
    printf("\t\t2.下载文件\n");
    printf("\t\t3.上传文件\n");
    printf("\t\t0.退出系统\n");
    printf("--------------------------------------------\n");
    printf("请选择你要执行的操作：\n");
}

void *recv_thread(void *arg) {
    int s_fd = *((int *) arg);
    MSG recv_msg = {0};
    while (1) {
        read(s_fd, &recv_msg, sizeof(MSG));
        if (recv_msg.type == MSG_TYPE_FILENAME) {
            printf("file in server:\n %s", recv_msg.buffer);
        } else if (recv_msg.type == MSG_TYPE_DOWNLOAD) {
            create_dir(filepath);
            strcat(filepath, "/");
            strcat(filepath, filename);
            write_file(filepath, recv_msg.buffer, sizeof(recv_msg.buffer));
        }
        memset(&recv_msg, 0, sizeof(MSG));
    }
}

void *upload(void *arg) {
    int s_fd = *((int *) arg);
    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_UPLOAD_DATA;
    send_file(s_fd, &send_msg, filepath);
}

void send_fun(int s_fd, MSG *send_msg) {
    int res = write(s_fd, send_msg, sizeof(MSG));
    if (res < 0) {
        perror("send msg error:");
    }
    memset(send_msg, 0, sizeof(MSG));
}

int main() {
    pthread_t send_thread_id, recv_thread_id;
    MSG send_msg = {0};
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    int news_fd = connect(s_fd, (struct sockaddr *) &address, sizeof(address));
    if (news_fd == -1) {
        printf("connect error\n");
        return 0;
    }
    pthread_create(&recv_thread_id, NULL, recv_thread, &s_fd);
    net_desk_ui();
    while (1) {
        char choice = getchar();
        switch (choice) {
            case '1':
                send_msg.type = MSG_TYPE_FILENAME;
                send_fun(s_fd, &send_msg);
                break;
            case '2':
                send_msg.type = MSG_TYPE_DOWNLOAD;
                printf("input download filename:");
                scanf("%s", filename);
                printf("input save dir:");
                scanf("%s", filepath);
                strcpy(send_msg.fname, filename);
                send_fun(s_fd, &send_msg);
                break;
            case '3':
                send_msg.type = MSG_TYPE_UPLOAD_FLAG;
                printf("input filepath:");
                scanf("%s", filepath);
                char *name = strrchr(filepath, '/');
                if (name == NULL) {
                    name = filepath;
                } else {
                    name++;
                }
                strcpy(send_msg.fname, name);
                send_fun(s_fd, &send_msg);
                pthread_create(&send_thread_id, NULL, upload, &s_fd);
                break;
            case '0':
                close(s_fd);
                return 0;
            default:
                net_desk_ui();
                break;
        }
    }
}