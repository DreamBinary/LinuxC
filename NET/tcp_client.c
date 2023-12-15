#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"

#define PORT 3333

int main() {
    int sockfd;
    int len;
    struct sockaddr_in address;
    int newsockfd;
    char buf[40];
    int len2;
    char rebuf[40];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    // address.sin_addr.s_addr = htonl(INADDR_ANY);
    // 自定义IP地址
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(PORT);
    len = sizeof(address);
    newsockfd = connect(sockfd, (struct sockaddr *) &address, len);
    if (newsockfd == -1) {
        printf("connect error\n");
        return 1;
    }

    printf("Enter a message to send to the server: \n");
    fgets(buf, sizeof(buf), stdin);

    len2 = sizeof(buf);
    send(sockfd, buf, len2, 0);
    recv(sockfd, rebuf, 256, 0);
    rebuf[sizeof(rebuf) + 1] = '\0';
    printf("receive from server: %s\n", rebuf);
    close(sockfd);
    return 0;
}