#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int s = 0;

void* addFun(void* args)
{
    int i = 0;
    for (; i < 1000000; i++)
        s++;
}

int main()
{
    pthread_t p1, p2;
    pthread_create(&p1, NULL, addFun, NULL);
    pthread_create(&p2, NULL, addFun, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("s = %d", s);
    return 0;
}