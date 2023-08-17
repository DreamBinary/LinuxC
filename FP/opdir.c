#include "dirent.h"
#include "stdio.h"
#include "unistd.h"

int main() {
    DIR* dir;
    DIR* tmp;
    struct dirent *ptr;
    int i;
    dir = opendir("test");
    tmp = dir;
    while ((ptr = readdir(dir)) != NULL) {
        printf("d_name: %s \n", ptr -> d_name);
    }
    printf("dir == tmp: %d\n", tmp == dir);
    closedir(dir);
}