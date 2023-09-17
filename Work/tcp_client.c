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
//    system("clear");//清空函数
    printf("=============TCP文件传输客户端=============\n");
    printf("------------------功能菜单------------------\n");
    printf("\t\t1.查询文件\n");
    printf("\t\t2.下载文件\n");
    printf("\t\t3.上传文件\n");
    printf("\t\t0.退出系统\n");
    printf("------------------------------------------------\n");
    printf("请选择你要执行的操作：\n");
}

void *thread_fun(void *arg) {
    int sockfd = *((int *) arg);
    MSG recv_msg = {0};
    while (1) {
        read(sockfd, &recv_msg, sizeof(MSG));
        if (recv_msg.type == MSG_TYPE_FILENAME) {
            printf("server path filename = %s\n", recv_msg.fname);
            memset(&recv_msg, 0, sizeof(MSG));
        } else if (recv_msg.type == MSG_TYPE_DOWNLOAD) {
            if (access("download", F_OK) == -1) {
//                printf("download not exist\n");
                int res = mkdir("download", 0777);
                if (res == -1) {
                    perror("mkdir() error");
                }
            }
            int fd = open("download/file", O_WRONLY | O_CREAT | O_TRUNC, 0777);
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

int main() {
    int sockfd;
    int res;
    char choice;
    struct sockaddr_in address;
    pthread_t thread_id;
    MSG send_msg = {0};
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    int newsockfd = connect(sockfd, (struct sockaddr *) &address, sizeof(address));
    if (newsockfd == -1) {
        printf("connect error\n");
        return 1;
    }
    pthread_create(&thread_id, NULL, thread_fun, &sockfd);
    net_desk_ui();
    while (1) {
        choice = getchar();
        switch (choice) {
            case '1':
                send_msg.type = MSG_TYPE_FILENAME;
                res = write(sockfd, &send_msg, sizeof(MSG));
                if (res < 0) {
                    perror("send msg error:");
                }
                memset(&send_msg, 0, sizeof(MSG));
                break;
            case '2':
                send_msg.type = MSG_TYPE_DOWNLOAD;
                res = write(sockfd, &send_msg, sizeof(MSG));
                if (res < 0) {
                    perror("send msg error:");
                }
                memset(&send_msg, 0, sizeof(MSG));
                break;
            case '3':

            case '0':
                exit(0);
            default:
//                printf("input error\n");
                break;
        }
        net_desk_ui();
    }
//
//    close(sockfd);
//    return 0;
}