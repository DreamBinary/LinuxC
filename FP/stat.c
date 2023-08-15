#include "sys/stat.h"
#include "unistd.h"
#include "stdio.h"

int main(){
    struct stat buf;
    stat("tmp.txt", &buf);
    printf("tmp.txt file size = %ld \n", buf.st_size);
    printf("tmp.txt file owner UID = %d \n", buf.st_uid);
    return 0;
}