#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* describes the pthread_exit(void *retval)  

*/

void* printHello(void* args)
{
    printf("this is child thread %ld ! \n", pthread_self());
    // pthread_self() => type is pthread_t (unsigned long type)
    //                   which print out the thread ID;
}

int main()
{   
    pthread_t pth1; 
    pthread_create(&pth1,NULL, printHello, NULL);
    printf("this is main thread %ld ! \n", pthread_self());
    printf("main thread is exiting\n");
    pthread_exit(NULL);
    // printf("main thread is exiting\n");  //-> this line would not be executed as the main thread is exit 
    return 0;
}
