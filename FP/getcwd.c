#include "stdio.h"
#include "unistd.h"
#include "limits.h"

int main() {
    char a[PATH_MAX];

    if (getcwd(a, PATH_MAX) == NULL) {
        perror("getcwd() error");
        return -1;
    } else {
        printf("Current working directory: %s\n", a);
        return 0;
    }
}