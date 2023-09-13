#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "unistd.h"
#include "pthread.h"
#include "dirent.h"
#include "string.h"

#define MSG_TPYE_FILENAME  1
#define PORT 3333
typedef struct msg {
    int type;//协议类型  0表示登录协议包  1文件传输包
    int flag;
    char buffer[128];//存放文件名外的内容
    char fname[50];//如果type1是文件名传输包那么fname里就放文件名
} MSG;//结构体随业务需求变化增添新的字段

void search_server_dir(int accept_socket) {
    struct dirent *dir = NULL;
    //opendir是打开Linux目录的api函数
    MSG info_msg = {0};
    int res = 0;
    DIR *dp = opendir("/tmp/");
    info_msg.type = MSG_TPYE_FILENAME;
    if (NULL == dp) {
        perror("open dir error");
        return;
    }
    while (1) {
        dir = readdir(dp);
        if (NULL == dir) {
            break;
        }
        if (dir->d_name[0] != '.') {
            //printf("name = %s\n", dir->d_name);
            memset(info_msg.fname, 0, sizeof(info_msg.fname));
            strcpy(info_msg.fname, dir->d_name);
            printf("filename: %s\n", info_msg.fname);
            res = write(accept_socket, &info_msg, sizeof(MSG));
            //把每个文件名拷贝到MSG结构体中，用套接字socket发送
            if (res < 0) {
                perror("send client error:");
                return;
            }
        }
    }
}

//创建线程函数
void *thread_fun(void *arg) {
    int acpt_socket = *((int *) arg);
    int res;
    char buffer[50] = {0};
    search_server_dir(acpt_socket);
    printf("finish send info of dir\n");
    while (1) {
        res = read(acpt_socket, buffer, sizeof(buffer));
        printf("client read %s\n", buffer);
        write(acpt_socket, buffer, res);
        memset(buffer, 0, sizeof(buffer));
        //服务器收到客户端数据后，原封不动的把数据回给客户端
    }
}

int main() {
//    char sendbuf[256];
    char buf[256];
    int s_fd, c_fd;
    int s_len, c_len;
    struct sockaddr_in s_addr, c_addr;
    pthread_t thread_id;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(PORT);
    s_len = sizeof(s_addr);
    bind(s_fd, (struct sockaddr *) &s_addr, s_len);
    listen(s_fd, 10);
    while (1) {
        printf("server waiting:\n");
        printf("server addr: %s\n", inet_ntoa(s_addr.sin_addr));
        c_len = sizeof(c_addr);
        c_fd = accept(s_fd, (struct sockaddr *) &c_addr, &c_len);
        printf("client addr: %s\n", inet_ntoa(c_addr.sin_addr));
//        recv(c_fd, buf, 256, 0);
//        buf[sizeof(buf) + 1] = '\0';
//        printf("receive from client: %s\n", buf);
//        printf("Enter a message to return the client: \n");
//        fgets(sendbuf, sizeof(sendbuf), stdin);
//        send(c_fd, sendbuf, sizeof(sendbuf), 0);

        pthread_create(&thread_id, NULL, thread_fun, &c_fd);
    }
}