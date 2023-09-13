#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"

#define MSG_TPYE_FILENAME  1
#define PORT 3333

typedef struct msg {
    int type;//协议类型  0表示登录协议包  1文件传输包
    int flag;
    char buffer[128];//存放文件名外的内容
    char fname[50];//如果type1是文件名传输包那么fname里就放文件名
} MSG;//结构体随业务需求变化增添新的字段

int main() {
    int sockfd;
    int len;
    struct sockaddr_in address;
    int newsockfd;
    MSG recv_msg = {0};
//    char buf[40];
//    int len2;
//    char rebuf[40];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    // 自定义IP地址
//    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(PORT);
    len = sizeof(address);
    newsockfd = connect(sockfd, (struct sockaddr *) &address, len);
    if (newsockfd == -1) {
        printf("connect error\n");
        return 1;
    }

//    printf("Enter a message to send to the server: \n");
//    fgets(buf, sizeof(buf), stdin);
//    len2 = sizeof(buf);
//    send(sockfd, buf, len2, 0);
//    recv(sockfd, rebuf, 256, 0);
//    rebuf[sizeof(rebuf) + 1] = '\0';
//    printf("receive from server: %s\n", rebuf);

    while (1) {
        read(sockfd, &recv_msg, sizeof(MSG));
        if (recv_msg.type == MSG_TPYE_FILENAME) {
            printf("server path filename = %s\n", recv_msg.fname);
            memset(&recv_msg, 0, sizeof(MSG));
        }
    }

//    close(sockfd);
    return 0;
}