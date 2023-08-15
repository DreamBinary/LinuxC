#include "stdio.h"


int main(int argc, char *argv[]){
    if (argc < 3) {
        printf("usages: %s oldpath newpath\n", argv[0]);
    }
    if (rename(argv[1], argv[2]) < 0) {
        perror("failed rename");
        return 1;
    } else {
        printf("%s => %s\nsuccessful!\n", argv[1], argv[2]);
    }
    return 0;
}