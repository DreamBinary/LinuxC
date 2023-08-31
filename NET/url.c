#include "netdb.h"
#include "stdio.h"
int main() {
    char* path = "www.baidu.com";
    struct hostent* host = gethostbyname(path);
    if (host == NULL) {
        printf("gethostbyname error\n");
        return 1;
    }
    printf("host name: %s\n", host->h_name);
    printf("host addr type: %d\n", host->h_addrtype);
    printf("host addr length: %d\n", host->h_length);
    printf("host addr list: %s\n", host->h_addr_list[0]);
    return 0;
}