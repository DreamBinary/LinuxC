/*****************************
Created by CXQ on 2023/9/13.
*****************************/

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>

#define MSG_TYPE_LOGIN  0
#define MSG_TPYE_FILENAME  1

typedef struct msg {
    int type;//协议类型  0表示登录协议包  1文件传输包
    int flag;
    char buffer[128];//存放文件名外的内容
    char fname[50];//如果type1是文件名传输包那么fname里就放文件名
} MSG;//结构体随业务需求变化增添新的字段



//根据客户端业务需求，客户端查看服务器目录下的文件
//服务器设计功能把目录下文件名信息获取发送给客户端
//默认情况服务器目录设置为客户的/home/目录
//在Linux下对文件和目录进行读取并获取文件名
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
        if (NULL == dir)  //如果readdir函数返回是空值表示目录全部读取完成
        {
            break;
        }
        if (dir->d_name[0] != '.')//把隐藏文件屏蔽
        {
            //printf("name = %s\n", dir->d_name);
            memset(info_msg.fname, 0, sizeof(info_msg.fname));
            strcpy(info_msg.fname, dir->d_name);
            printf("%s\n", info_msg.fname);
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
    //read函数就是接收客户端发来的数据，返回值表示实际从客户端那边收到的字节数；
    //buffer：收到客户端数据后，把数据存放的位置（地址），sizeof(buffer)：希望读取的字节数
    search_server_dir(acpt_socket);
    printf("目录信息发送给客户端完成！\n");
    while (1) {
        res = read(acpt_socket, buffer, sizeof(buffer));
        printf("client read %s\n", buffer);
        write(acpt_socket, buffer, res);
        memset(buffer, 0, sizeof(buffer));
    }
}


int main() {
    char buffer[50] = {0};
    int res;
    //1.创建套接字描述符 买手机

    int server_socket;//socket网络描述符，，，套接字描述符
    server_socket = socket(AF_INET, SOCK_STREAM, 0);//想要向网络发送数据都是用server_socket这个套接字描述符
    int accept_socket;
    pthread_t thread_id;//线程编号
    //search_server_dir();
    printf("开始创建TCP服务器！\n");
    if (server_socket < 0) {
        perror("socket create failed");
        return 0;
    }
    //2.在告诉服务器ip和端口号 购买电话卡
    //要有一个保存ip地址和端口的变量  struct sockaddr_in server_addr;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;//ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY;//INADDR)_ANY告诉系统自动绑定网卡ip
    // printf("add : %s\n", inet_ntoa(server_addr.sin_addr));
    server_addr.sin_port = htons(6666);
    //端口号进行赋值时，不能直接赋整型，而是把主机字节顺序 转换成网络字节顺序
    // htons() 主机host t转 network short
    // nots()  network zhuan 主机


    // 如果服务器程序退出后又立刻打开，系统提示地址已经被使用
    //这是因为ip和端口是系统资源，必须设置为端口号可重复使用
    int optvalue = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));




//3.将设定好的ip和端口号绑定到我们的server_socket描述符上    将电话卡插到手机上
//bind()
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("server bind errror:");
        return 0;
    }
    //4.调用listen()开始监听程序 相当于把电话带在身上

    //后一个backlog>0就可以
    if (listen(server_socket, 10) < 0) {
        perror("server listen error:");
        return 0;
    }

    //5.以上4步都好后，可以等待客户端的连接

    //如果有客户连接，则返回一个新套接字
    //accept() 特点：由程序调用这个函数时，若没有客户端连接到我们的服务器上，那么这个
    //函数将被堵塞（程序停止），直到由客户端连接到服务器上，这个函数将解堵塞
    //并且返回一个新的套接字描述符，后期和客户端的通讯都将交给这个心得套接字描述符来负责
    printf("TCP服务器准备完成，等待客户端连接！\n");
    //将以下两行放入while循环内多线程
    //accept_socket = accept(server_socket, NULL, NULL);
    //printf("有客户端连接到服务器！\n");
    while (1) {
        accept_socket = accept(server_socket, NULL, NULL);
        printf("有客户端连接到服务器！\n");

        //创建一个新的进程，创建成功之后，系统就会执行thread_fun 多线程代码
        pthread_create(&thread_id, NULL, thread_fun, &accept_socket);




        /*
            //read函数就是接收客户端发来的数据，返回值表示实际从客户端那边收到的字节数；
            //buffer：收到客户端数据后，把数据存放的位置（地址），sizeof(buffer)：希望读取的字节数
            res = read(accept_socket, buffer, sizeof(buffer));
            printf("client read %s\n", buffer);
            write(accept_socket, buffer, res);
            memset(buffer, 0, sizeof(buffer));
            //服务器收到客户端数据后，原封不动的把数据回给客户端
        */
    }

    //使用pthread_create创建多线程
    printf("%s 向你问好!\n", "linux1");
    return 0;
}