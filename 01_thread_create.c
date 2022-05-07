#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*describes the pthread basic usage, as 
    1. pthread_create(), 
    2.pthread_self() 
    3. sleep for thread suspend;
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
    //method 1: let the main thread suspend for a while, then CPU time slice can give to child thread
    //sleep(3);  //wait for 3 second in Linux, and 3ms in Windows.

    //method2 : as main thread wait until child thread finished. 
     pthread_join(pth1, NULL);    
    return 0;
}
