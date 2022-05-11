#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


typedef struct 
{
    int num;
    int age;
} struct_t;

//struct_t st1;   //global variable

void* callback(void* args)
{
    printf("this is child thread %ld ! \n", pthread_self());
    // pthread_self() => type is pthread_t (unsigned long type)
    //                   which print out the thread ID;

     //struct_t st1;  //local variables only exist on current thread stack
    ((struct_t *)args)->num = 100;
    ((struct_t *)args)->age = 200;

    printf("args->num = %d,  args->age = %d", ((struct_t *)args)->num, ((struct_t *)args)->age); 
    //use the struct to return the values to thread which recycle current ones.
    pthread_exit(args);   //Note : be careful the modified value shall be ret back to Main thread.
    return NULL;
}

int main()
{   
    pthread_t pth1;
    struct_t st1 ={20, 10};
    pthread_create(&pth1,NULL, callback, &st1);
    printf("this is main thread %ld ! \n", pthread_self());
    printf("before detach, args->num = %d,  args->age = %d\n", st1.num, st1.age);
    pthread_detach(pth1); //子线程和主线程分离；主线程在退出后，不会释放地址空间；子线程任可以继续执行；
    printf("after detach,args->num = %d,  args->age = %d\n", st1.num, st1.age);
    pthread_exit(NULL);

    return 0;
}
