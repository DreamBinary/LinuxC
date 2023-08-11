#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
int main(){
    key_t key;
    int proj_id = 1;
    int msgid;

    char msg1[] = {"hello world"};
    char msg2[] = {"hello linux"};

    struct msgbuf{
        long mtype;
        char mtext[1024];
    }snd, rcv;

    key = ftok("/tmp", proj_id);
    if(key == -1){
        perror("ftok error");
        exit(1);
    }
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1){
        perror("msgget error");
        exit(1);
    }

    snd.mtype = 1;
    sprintf(snd.mtext, "%s", msg1);
    if (msgsnd(msgid, &snd, sizeof(msg1) + 1, 0) == -1){
        perror("msgsnd error");
        exit(1);
    }

    snd.mtype = 2;
    sprintf(snd.mtext, "%s", msg2);
    if (msgsnd(msgid, &snd, sizeof(msg2) + 1, 0) == -1){
        perror("msgsnd error");
        exit(1);
    }

    if (msgrcv(msgid, &rcv, 80, 1, IPC_NOWAIT) == -1){     // snd.mtype = 1;
        perror("msgrcv error");
        exit(1);
    }
    printf("the rcv msg: %s\n", rcv.mtext);

    system("ipcs -q");
    exit(0);
}