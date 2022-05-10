#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#define  MAX 50

int g_num;

void* addfunA(void* args)
{
    for (int i = 0; i < MAX; i++)
    {
        int cur = g_num;
        cur++;
        usleep(10);
        g_num = cur;
        printf("Thread A, id = %ld, num = %d\n", pthread_self(), g_num);
    }
    return NULL;
}

void* addfunB(void* args)
{
    for (int i = 0; i < MAX; i++)
    {
        int cur = g_num;
        cur++;
        usleep(10);
        g_num = cur;
        printf("Thread B, id = %ld, num = %d\n", pthread_self(), g_num);
        usleep(5);
    }
    return NULL;
}

int main()
{
    pthread_t p1, p2;
    pthread_create(&p1, NULL, addfunA, NULL);
    pthread_create(&p2, NULL, addfunB, NULL);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}