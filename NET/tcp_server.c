#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "unistd.h"

#define PORT 3333

int main() {
    char sendbuf[256];
    char buf[256];
    int s_fd, c_fd;
    int s_len, c_len;
    struct sockaddr_in s_addr, c_addr;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(PORT);
    s_len = sizeof(s_addr);
    bind(s_fd, (struct sockaddr*)&s_addr, s_len);
    listen(s_fd, 10);
    while (1) {
        printf("server waiting:\n");
        // printf("server addr: %s\n", inet_ntoa(s_addr.sin_addr));
        c_len = sizeof(c_addr);
        c_fd = accept(s_fd, (struct sockaddr*)&c_addr, &c_len);
        // printf("client addr: %s\n", inet_ntoa(c_addr.sin_addr));
        recv(c_fd, buf, 256, 0);
        buf[sizeof(buf) + 1] = '\0';
        printf("receive from client: %s\n", buf);
        printf("Enter a message to return the client: \n");
        fgets(sendbuf, sizeof(sendbuf), stdin);

        send(c_fd, sendbuf, sizeof(sendbuf), 0);
    }
}