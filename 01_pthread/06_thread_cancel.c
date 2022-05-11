#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* callback(void * args)
{
    for (int j = 0; j < 4; j++)
       printf(" j = %d\n", j);
    
    printf("current running is pth1, tid = %ld\n", pthread_self());

    for(int i=0; i<9; ++i)
    {
        printf(" child i: %d\n", i);
          printf("current running is pth1, tid = %ld\n", pthread_self());
    }

    return NULL;
}



int main(int argc, char** argv)
{
    pthread_t pth1;
    pthread_create(&pth1, NULL, callback, NULL);

    printf("pth1 is created successfully tid = %ld\n", pth1);

    // for main thread will execute below cmd;
    printf("this is main tid = %ld", pthread_self());
    for (int i = 0; i < 3; i++)
        printf(" i = %d\n", i);

    // kill pth1 at main tid;
    int k= pthread_cancel(pth1);
    if(k !=0)
    {
        perror("pthread_cancel");
    }
    else
    {
        printf("k =  %d\n", k);
    }

    // let main tid quit;
    pthread_exit(NULL);
    return 0; 
}