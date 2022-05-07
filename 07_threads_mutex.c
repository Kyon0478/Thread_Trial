/*
1. in order to avoid the dead-lock, the pthread_mutex_lock() and pthread_mutex_unlock() shall be paired.
2. There would be return in the middle of the function, if the pthread_mutex_unlock() is after the return, the critical area would be locked forever;
3. function A call another function B, if there is pthread_mutex_lock() in both functionA and functionB, so it would also get dead_lock;
4. how many mutex_lock shall be set depends on how many critical area shall be protected, rather than how many threads are there. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#define  MAX 50

int g_num;
// define a mutex lock; 
pthread_mutex_t lock;

void* addfunA(void* args)
{
    for (int i = 0; i < MAX; i++)
    {   
        pthread_mutex_lock(&lock);
        int cur = g_num;
        cur++;
        usleep(10); // usleep is ms;
        g_num = cur;
        printf("Thread A, id = %ld, num = %d\n", pthread_self(), g_num);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

void* addfunB(void* args)
{
    for (int i = 0; i < MAX; i++)
    {
        pthread_mutex_lock(&lock);
        int cur = g_num;
        cur++;
        g_num = cur;
        printf("Thread B, id = %ld, num = %d\n", pthread_self(), g_num);
        pthread_mutex_unlock(&lock);
        usleep(5);
    }
    return NULL;
}

int main()
{   
    pthread_t p1, p2;
 
    // initialize the mutex lock;
    // pthread_mutex_init(pthread_mutex_t *restrict mutex,
    //                    const pthread_mutexattr_t *restrict attr);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&p1, NULL, addfunA, NULL);
    pthread_create(&p2, NULL, addfunB, NULL);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    // counterpart as mutex init;
    pthread_mutex_destroy(&lock);

    return 0;
}