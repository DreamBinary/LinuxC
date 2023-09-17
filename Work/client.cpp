/*****************************
// Created by CXQ on 2023/9/13.
*****************************/
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MSG_TPYE_FILENAME  1


typedef struct msg {
    int type; //协议类型  0表示登录协议包  1文件名传输包
    int flag;
    char buffer[128];//存放文件名外的内容
    char fname[50]; //如果type是1就是文件名传输包，那么fname中放的就是文件名
} MSG;//会根据业务需求的变化添加新的字段

void net_desk_ui() {
    system("clear");//清空函数
    printf("=============TCP文件传输客户端=============\n");
    printf("------------------功能菜单------------------\n");
    printf("\t\t1.查询文件\n");
    printf("\t\t2.下载文件\n");
    printf("\t\t3.上传文件\n");
    printf("\t\t0.退出系统\n");
    printf("------------------------------------------------\n");
    printf("请选择你要执行的操作：\n");
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;//用来填写你要链接到服务器的ip地址和端口号
    MSG recv_msg = {0};
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("client socket faile:");
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    //如果服务和客户端都是在同一台那么ip可设为127.0.0.1
    server_addr.sin_port = htons(6666);
    //创建好套接字之后，客户端要连接到服务器，这个时候链接服务器使用connect
    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect error!");
        return 0;
    }

    printf("客户端连接服务器成功！\n");

    net_desk_ui();

    while (1) {
        read(client_socket, &recv_msg, sizeof(MSG));
        if (recv_msg.type == MSG_TPYE_FILENAME) {
            printf("server path filename = %s\n", recv_msg.fname);
            memset(&recv_msg, 0, sizeof(MSG));
        }
    }
    //我们用户在客户端连续输入字符串。回车表示把数据发送
    /*
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        res = write(client_socket, buffer, sizeof(buffer));
        printf("send byte = %d\n", res);
        memset(buffer, 0, sizeof(buffer));
        res = read(client_socket, buffer, sizeof(buffer));
        printf("recv from server info: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    */
    printf("%s 向你问好!\n", "linux2");
    return 0;
}