#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread1()
{
    pthread_detach(pthread_self());//分离后仍可被等待，此行不是必须
    printf("pid is: %d, tid is: %d\n", getpid(),pthread_self());  //两个pid一样
    return (void*)1;
}
int main()
{
    pthread_t tid;
    void *ret;
    int err = pthread_create(&tid, NULL, thread1, NULL);//创建线程
    if (err != 0)
    {
        perror("pthread_create\n");
        return err;
    }
    //如果直接运行等待代码，一般会等待成功，返回1
    //如果在等待之前加入取消。等待错误，返回-1
    //  pthread_cancel(tid);
    //线程可以自我取消也可以被取消，线程终止
    //调用pthread_exit(tid);和取消同样用法。
    int tmp = pthread_join(tid, &ret); //等待线程运行结束
    if (tmp == 0)
    {
        printf("wait success\n");
    }
    else
    {
        printf("wait failed\n");
    }
    printf(" pid is: %d, tid is: %d\n", getpid(),pthread_self());
    sleep(1);
    return 0;
}
