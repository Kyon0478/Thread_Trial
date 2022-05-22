#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* thread exit, system would recycle the current thread related kernel resource 
   and as well as the thread would release the stack on user space 
   pthread_exit(void *retval)  who recycle the thread, who would get the retval.
*/
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
    //use the struct to return the values to thread which recycle current ones.
    pthread_exit(args);   //Note : be careful the modified value shall be ret back to Main thread.
    return NULL;
}

int main()
{   
    pthread_t pth1;
    struct_t st1;
    pthread_create(&pth1,NULL, callback, &st1);
    printf("this is main thread %ld ! \n", pthread_self());
    // pthread_join() to get the child thread return values;
    // 1. if the 2nd arg is NULL, which means doesnt receive any retval from child thread.
    // pthread_join(pth1, NULL);
    // 2. ptr is 二级指针， which point to the child thread retval;
    void *ptr;
    pthread_join(pth1, &ptr);  //ptr would point to the &st1

    printf("num is %d, age is %d", ((struct_t *)ptr)->num, ((struct_t *)ptr)->age);

    return 0;
}
