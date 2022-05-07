#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int first;
    int last;
    int id; 
} my_args_t;

int arr[5000];
int g_s1 = 0, g_s2 = 0;

//describes the pthread basic usage, as pthread_create(), and pthread_join();
//pthread_create()
//  1. thread -> point to a structure of type pthread_t; pass it to pthread_create and in order to initialize it;
//  2. attr -> specify any attribute this thread may have, eg. incl. setting stack size or info. about the scheduling priority of the thread. \
           or it can be simply pass with NULL in;
//  3. arg. -> most complex, is function pointer, function name (start_routine) which is passed a single argument of type void* ,returns a value of type void *
//  4. arg -> is the argument to be passed to the function where thread begins execution

void* addfunc(void* args)
{
    int s = 0;
    for (int i = (int)((my_args_t*)args)->first; i < (int)((my_args_t*)args)->last; i++)
    {
        s = s + arr[i];
    }
    printf("s = %d \n", s);
}

void* smartAdd(void* args)
{
    for (int i = 0; i < 5000; i++)
    {
        if ((int)(((my_args_t *)args)->id) == 1)
        {
            g_s1 = g_s1 + arr[i];
        }
        else
        {
            g_s2 = g_s2 + arr[i];
        }
    }
    printf("g_s1 = %d , g_s2 =%d \n", g_s1, g_s2);
}


int main()
{
    srand(time(NULL));
    for (int i = 0; i < 5000; i++)
        arr[i] = rand() % 50;

    //for (int i = 0; i < 5000; i++)
    //    printf("arr[%d] = %d \n", i, arr[i]);

    pthread_t p1;
    pthread_t p2;
    my_args_t arg1 ={0, 2500, 1};
    my_args_t arg2 ={2500, 5000, 2};
    pthread_create(&p1, NULL, addfunc,  &arg1); //the 4th argument can be passed to the func
    pthread_create(&p2, NULL, addfunc,  &arg2);  
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}
