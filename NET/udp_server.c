#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "unistd.h"

#define PORT 3333

int main() {
    char buf[256];
    int s_fd, c_fd;
    int c_len;
    struct sockaddr_in s_addr, c_addr;
    s_fd = socket(AF_INET, SOCK_DGRAM, 0);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(PORT);
    bind(s_fd, (struct sockaddr *) &s_addr, sizeof(s_addr));
    while (1) {
        printf("server waiting:\n");
        printf("server IP: %s\n", inet_ntoa(s_addr.sin_addr));
        c_len = sizeof(c_addr);
        recvfrom(s_fd, buf, sizeof(buf), 0, (struct sockaddr *) &c_addr, &c_len);
        buf[sizeof(buf) + 1] = '\0';

        printf("client IP: %s\n", inet_ntoa(c_addr.sin_addr));
        printf("receive from client: %s\n", buf);
    }
}