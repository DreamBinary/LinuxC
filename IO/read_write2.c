#include "stdio.h"

int main() {
    char* path = "tmp.txt";

    FILE* fp = fopen(path, "r");
    if (fp != NULL) {
        printf("open success\n");
    } else {
        printf("open error\n");
        return 1;
    }
    FILE* fpw = fopen("tmp2.txt", "w");
    int r;
    if (fseek(fpw, 0L, SEEK_END) != 0) {
        printf("fseek error\n");
        return 1;
    }
    while ((r = fgetc(fp)) != EOF) {
        printf("%c", r);
        fputc(r, fpw);

        if (ferror(fp)) {
            printf("read error\n");
            return 1;
        }
    }

    int result = fclose(fp);
    if (result == 0) {
        printf("close success\n");
    } else if (result == EOF) {
        printf("close error\n");
        return 1;
    }
}