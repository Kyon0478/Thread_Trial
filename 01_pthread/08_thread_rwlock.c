#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX 50
#define RD_THREAD_NUM 5
#define WR_THREAD_NUM 3

pthread_mutex_t  lock; 
pthread_rwlock_t rwlock;
int g_num; 
/*Note:
 1. either pthread_rwlock_rdlock() or pthread_rwlock_wrlock(), can not be set simultaneously/
 2. pthread_rwlock_rdlock() is shareable, which means the critical area can be read by multi-thread simultaneously
 3. pthread_rwlock_wrlock() is exclusive, can not be shared.
 4. pthread_rwlock_trylock(pthread_rwlock_t *rwlock); 
*/
void* read_Num(void* args)
{
    for (int i = 0; i < MAX ; i++)
    {  
        pthread_rwlock_rdlock(&rwlock);  //rd -> readlock
        // pthread_self() is unsigned long int; 
        printf("Thread_Read_ID = %ld,  g_num =%d\n", pthread_self(), g_num);
        pthread_rwlock_unlock(&rwlock); // unlock func is the same for both rdlock and wrlock
    }
    usleep(rand() % RD_THREAD_NUM); // ask the thread sleep for related (0..4 )ms ;
} 

void* write_Num(void* args)
{
    for (int i = 0; i < MAX; i++)
    {   
        pthread_rwlock_wrlock(&rwlock);  //wr -> writelock
        g_num++;
        printf("Thread_Write_ID = %ld , g_num = %d\n", pthread_self(), g_num);
        pthread_rwlock_unlock(&rwlock);
    }
    usleep(5);
}

int main()
{
    //Init rwlock 
    pthread_rwlock_init(&rwlock, NULL);
    
    //create an array for 5 rd thread, 3 for wr thread.
    pthread_t p1[RD_THREAD_NUM], p2[WR_THREAD_NUM];

    for (int i = 0; i < RD_THREAD_NUM; i++)
        pthread_create(&p1[i], NULL, read_Num, NULL);

    for (int i = 0; i < WR_THREAD_NUM; i++)
        pthread_create(&p2[i], NULL, write_Num, NULL);
    
    //thread block, resource recycle
    for (int i = 0; i < RD_THREAD_NUM; i++)
        pthread_join(p1[i], NULL);
    for (int i = 0; i < WR_THREAD_NUM; i++)
        pthread_join(p2[i], NULL);
    
    //destory rwlock
    pthread_rwlock_destroy(&rwlock);

    return 0;
}