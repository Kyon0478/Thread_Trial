#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
int s = 0;

void* addFun(void* args)
{  
    pthread_mutex_lock(&lock);
    int i = 0;
    for (; i < 100000; i++)
    {  
        s++;   
    }
    pthread_mutex_unlock(&lock);
}

int main()
{
    pthread_t p1, p2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&p1, NULL, addFun, NULL);
    pthread_create(&p2, NULL, addFun, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("s = %d", s);
    return 0;
}