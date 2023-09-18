/*****************************
Created by CXQ on 2023/9/18.
*****************************/

#include "pthread.h"
#include "stdio.h"

int cnt = 100;
pthread_mutex_t mutex;

void *pa(void *arg) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex);
        int x = cnt;
        x = x - 1;
        cnt = x;
        printf("pa: cnt = %d\n", cnt);
        pthread_mutex_unlock(&mutex);
    }
}

void *pb(void *arg) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex);
        int x = cnt;
        x = x + 1;
        cnt = x;
        printf("pb: cnt = %d\n", cnt);
        pthread_mutex_unlock(&mutex);
    }
}


int main() {
    pthread_t tid1, tid2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&tid1, NULL, pa, NULL);
    pthread_create(&tid2, NULL, pb, NULL);
//    pthread_join(tid1, NULL);
//    pthread_join(tid2, NULL);
    printf("cnt = %d\n", cnt);
    sleep(1);
    printf("cnt = %d\n", cnt);
    return 0;
}