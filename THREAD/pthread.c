//
// Created by CXQ on 2023/9/11.
//

//#include <stdio.h>
//#include <pthread.h>
//
//void *hello(void *arg) {
//    printf("Hello, World!\n");
//    return NULL;
//}
//
//int main() {
//    pthread_t id;
//    int ret = pthread_create(&id, NULL, (void *) &hello, NULL);
//    if (ret != 0) {
//        printf("pthread_create error: ret = %d\n", ret);
//        return 1;
//    }
////    sleep(1);
//    printf("This is main thread.\n");
//    pthread_join(id, NULL);
//    return 0;
//}

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#define MAX 10

pthread_t thread[2];
pthread_mutex_t mut;
int number = 0, i;

void thread1() {
    printf("thread1 : I'm thread 1,starting...\n");

    for (i = 0; i < MAX; i++) {
        printf("thread1 : number = %d\n", number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(2);
    }

    pthread_exit(NULL);
}

void thread2() {
    printf("thread2 : I'm thread 2,starting...\n");

    for (i = 0; i < MAX; i++) {
        printf("thread2 : number = %d\n", number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(4);
    }

    pthread_exit(NULL);
}

void thread_create(void) {
    memset(&thread, 0, sizeof(thread));     //comment1
    /*创建线程*/
    if (pthread_create(&thread[0], NULL, (void *) thread1, NULL) != 0)  //comment2
        printf("create thread1 failed!\n");
    else
        printf("create thread1 successed!\n");

    if (pthread_create(&thread[1], NULL, (void *) thread2, NULL) != 0)  //comment3
        printf("create thread failed!\n");
    else
        printf("create thread2 successed!\n");
}

void thread_wait(void) {
    /*等待线程结束*/
    if (thread[0] != 0) {
        //comment4
        pthread_join(thread[0], NULL);
        printf("thread1 end !\n");
    }
    if (thread[1] != 0) {
        //comment5
        pthread_join(thread[1], NULL);
        printf("thread2 end !\n");
    }
}

int main() {
    /*用默认属性初始化互斥锁*/
    pthread_mutex_init(&mut, NULL);
    thread_create();
    thread_wait();

    return 0;
}

