//
// Created by fivv on 2023/10/7.
//

#ifndef __MSG_H
#define __MSG_H

#define MSG_TYPE_FILENAME  1
#define MSG_TYPE_DOWNLOAD  2
#define MSG_TYPE_UPLOAD_FLAG 3
#define MSG_TYPE_UPLOAD_DATA 4
#define PORT 3333
typedef struct msg {
    int type;//协议类型  0表示登录协议包  1文件传输包
    int flag;
    char buffer[2048];//存放文件名外的内容
    char fname[50];//如果type1是文件名传输包那么fname里就放文件名
    int bytes;
} MSG;//结构体随业务需求变化增添新的字段

#endif //__MSG_H
