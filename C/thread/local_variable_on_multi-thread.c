/*****************************************************************
这个程序验证了循环申请的本地变量t, 它的地址可能不变的,若在多线程里面,
则多线线程公用对一个地址的数据,进行读写,这是有问题的

*****************************************************************/

#include <stdio.h>
#include <pthread.h>

struct test{
    int a;
    int b;
    int c;
    int d;
    int e;
};


void print_message(void *ptr)
{
    int *t;
    t = (int *)ptr; 

    printf("message: %d\n", *t);
    printf("message address: : %p\n", t);
    printf("=============\n");
    pthread_exit((void *)1);
}

//测试多线程,循环传递变量问题
int thread_test(pthread_t * pthread, int *t)
{
    int ret = 0;

    ret = pthread_create(pthread, NULL, (void *)&print_message, (void*)t);
    if (ret != 0) {
        printf("thread %d\n", (int)*pthread);
        perror("pthread create");
    }

    return ret;
}

int main(int argc, char **argv)
{
    char *msg_a = "hello world";
    char *msg_b = "I am comming";
    void *reatval;
    int ret;
    //struct test t[16];
    int i;
    pthread_t pthreads[16];

    do {
        for (i = 0; i < 16; i++) {

        /*这里就是用数组,和单个变量的区别*/
#if 0
            t->a = 1;
            t->b = i;  
            t->c = 3;
            t->d = 4;
            t->e = 5;

            thread_test(&pthreads[i], &t[i]); 
#else 
            int t = i;
            ret = thread_test(&pthreads[i], &t); 
            if (ret != 0) {
                break;
            }
#endif
        }

    }while (0);

    for (i=0; i < 16; i++) {
        ret = pthread_join(pthreads[i], &reatval);
        //   printf("pthrad a is %d\n", ret);
        //   printf("pthrad a value is %d\n", (int)(reatval));
        if (ret != 0) {
            perror("pthread join");
            return -1;
        }

    }
}
