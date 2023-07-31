#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"

void* fun(void*);

int main() {
    pthread_t id;

    // int res = pthread_create(&id, NULL, fun, NULL);
    int arg = 123456;
    // int res = pthread_create(&id, NULL, fun, (void*)&arg);
    int res = pthread_create(&id, NULL, fun, &arg);
    printf("thread id %ld\n", id);
    printf("thread res %d\n", res);
    printf("main start\n");

    for (int i = 0; i < 3; i++) {
        printf("main running\n");
        sleep(1);
    }
    printf("main over\n");
    exit(0);
}

void* fun(void* arg) {
    printf("fun start\n");
    if (arg != NULL) {
        printf("arg -> %d\n", *(int*)arg);
        // printf("arg -> %d\n", *(char*)arg);
    }

    for (int i = 0; i < 5; i++) {
        printf("fun running\n");
        sleep(1);
    }
    printf("fun over\n");
}