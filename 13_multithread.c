#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

//describes the pthread basic usage, as pthread_create(), and pthread_join();
//pthread_create()
//  1. thread -> point to a structure of type pthread_t; pass it to pthread_create and in order to initialize it;
//  2. attr -> specify any attribute this thread may have, eg. incl. setting stack size or info. about the scheduling priority of the thread. \
           or it can be simply pass with NULL in;
//  3. arg. -> most complex, is function pointer, function name (start_routine) which is passed a single argument of type void* ,returns a value of type void *
//  4. arg -> is the argument to be passed to the function where thread begins execution

void* printHello(void* args)
{
    //printf("hello world 2022 \n");
    for (int i = 0; i < 100; i++)
        printf("this is %s running, value is %d \n", (char*)args, i);
}

int main()
{
    pthread_t p1;  //pthread_t  unsigned long type/
    pthread_t p2;
    pthread_create(&p1,NULL,  printHello, "p1");  //the 4th argument can be passed to the func
    pthread_create(&p2, NULL, printHello, "p2");  
    
    pthread_join(p1, NULL);
    sleep(1);
    pthread_join(p2, NULL);
    return 0;
}
