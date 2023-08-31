#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "unistd.h"

#define PORT 3333

int main() {
    int sockfd;
    int len;
    struct sockaddr_in address;
    char buf[40];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(PORT);
    len = sizeof(address);
    printf("Enter a message to send to the server: \n");
    fgets(buf, sizeof(buf), stdin);
    sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&address, len);
    printf("send success\n");
    return 0;
}