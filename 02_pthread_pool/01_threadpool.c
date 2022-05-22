/*
 * @Author: your name
 * @Date: 2022-05-10 20:10:59
 * @LastEditTime: 2022-05-22 14:28:44
 * @LastEditors: Kyon0478 zhangcheng0478@163.com
 * @Description:  https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \200_Process_Thread\Thread_Trial\02_pthread_pool\01_threadpool.c
 */

#include "01_threadpool.h"


const int NUMBER = 2;

//initialize pthread pool
threadpool_t* threadPoolCreate(int qsize, int minN, int maxN)
{
    //queueSize  => 
    //minNum / maxNum => 
    //create a pool on heap;
    threadpool_t* pool = (threadpool_t*)malloc(sizeof(threadpool_t));
    do
    {
        if(pool == NULL)
        {
            printf("malloc threadpool is failed .. \n");
            break;
        }
        
        pool->threadID = (pthread_t*)malloc(sizeof(pthread_t)*maxN);
        if (pool->threadID == NULL)
        {
            printf("malloc threadID is failed.. \n");
            break;
        }

        memset(pool->threadID, 0, sizeof(pthread_t)*maxN);
        pool->maxNum = maxN;
        pool->minNum = minN;
        pool->busyNum = 0;
        pool->aliveNum = minN;  //Equals to the minum Number
        pool->exitNum  = 0 ;

        if(pthread_mutex_init(&pool->mutexPool, NULL) !=0 ||
           pthread_mutex_init(&pool->mutexBusy, NULL) !=0 ||
           pthread_cond_init(&pool->notEmpty,   NULL) !=0 ||
           pthread_cond_init(&pool->notFull,    NULL) !=0 )
        {
            printf("mutex init failed ... \n");
        } 
      
        pool->taskQ = (task_t*)malloc(sizeof(task_t) * qsize);
        pool->queueCapacity = qsize;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear =0;
        pool->shutdown = 0; 

        //create manager pthread;
        pthread_create(&pool->managerID, NULL, manager, pool);
        //create work pthread;
        for(int i = 0; i< minN; ++i)
        {  
            pthread_create(&pool->threadID[i], NULL, worker, pool);
        }
        //create pool successfully, return pool;
        return pool;
    } while (0);

     //release resource
     if (pool->threadID && pool)    free(pool->threadID);
     if (pool->taskQ && pool) free(pool->taskQ);
     if (pool)  free(pool);

    return NULL;
}

//worker func
void *worker(void* args)
{
   threadpool_t* pool = (threadpool_t *)args;
   while(1)
   {
       pthread_mutex_lock(&pool->mutexPool);
       //check taskQ whether is empty;
       if (pool->queueSize == 0 && !pool->shutdown)
       {
           //block work thread;
           pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

           //check whether to destroy the pthread;
           if(pool->exitNum > 0)
           {
               pool->exitNum--;
               if(pool->aliveNum > pool->minNum)
               {
                    pool->aliveNum--;
                    //need to unlock the mutexPool before pthread_exit, otherwise it would get deadlock;
                    pthread_mutex_unlock(&pool->mutexPool);
                    //replaced with pthreadExit(pool)
                    //pthread_exit(NULL);
                    threadExit(pool);
               }
           }
       }
       //check the pool is shutdown
       if(pool->shutdown)
       {
           pthread_mutex_unlock(&pool->mutexPool);
           //pthread_exit(NULL);
           threadExit(pool);
       }

       //Consumer from the threadpool
       task_t  task; 
       task.func =  pool->taskQ[pool->queueFront].func;
       task.arg  =  pool->taskQ[pool->queueFront].arg;
       // task queue as a circle; 
       // move the head node; 
       pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
       pool->queueSize --; 
       //wakeup producer; 
       pthread_cond_signal(&pool->notFull);

       pthread_mutex_unlock(&pool->mutexPool);
       
       //execute the task ; 
       printf("thread %ld start working... \n", pthread_self());
       pthread_mutex_lock(&pool->mutexBusy);
       pool->busyNum++;
       pthread_mutex_unlock(&pool->mutexBusy);
       task.func(task.arg);

       //free task arg;
       free(task.arg);
       task.arg = NULL;
       printf("thread %ld end working... \n", pthread_self());
       //end the task;
       pthread_mutex_lock(&pool->mutexBusy);
       pool->busyNum--;
       pthread_mutex_unlock(&pool->mutexBusy);
   }

   return NULL;
}

//manager func
void *manager(void* args)
{
    threadpool_t* pool = (threadpool_t*)args;
    while(!pool->shutdown)
    {
         //no need to check too frequently , set as 3s;
         sleep(3);
         // 
         pthread_mutex_lock(&pool->mutexPool);
         int qSize =  pool->queueSize; 
         int aliveN = pool->aliveNum;
         //int busyN = pool->BusyNum ; //also can be added here, as the lock is for entire pool;
         pthread_mutex_unlock(&pool->mutexPool);

         pthread_mutex_lock(&pool->mutexBusy);
         int busyN  = pool->busyNum;
         pthread_mutex_unlock(&pool->mutexBusy);

         //add thread.
         if (qSize > aliveN && aliveN < pool->maxNum)
         {  
             pthread_mutex_lock(&pool->mutexPool);
             int counter = 0;
             for(int i = 0; i < pool->maxNum && counter < NUMBER & pool->aliveNum < pool->maxNum; ++i)
             {
                 if(pool->threadID[i]==0)
                 {
                     pthread_create(&pool->threadID[i], NULL, worker, pool);
                     counter++;
                     pool->aliveNum++;
                 }
             }
            pthread_mutex_unlock(&pool->mutexPool);
         }

         //destroy pthread; 
         //busyThread * 2 < aliveThread && aliveThread > minThread;
        if(busyN*2 < aliveN && aliveN > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER; //once destroy 2 pthreads;
            pthread_mutex_unlock(&pool->mutexPool);

            //ask thread to be killed; 
            for(int i = 0; i < NUMBER; ++i)
            {  
                //pthread_cond_signal == pthread_cond_broadcast here , as only 1 thread can get the lock::mutexPool
                //then it can go to the next steps;
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }   
}

//destory pthread pool
void threadExit(threadpool_t * pool)
{
    pthread_t tid = pthread_self();
    for(int i = 0; i < pool->maxNum; ++i)
    {
        if (pool->threadID[i] ==tid)
        {
            pool->threadID[i] = 0;
            printf("threadExit() called, %ld is exiting..\n", tid);
        }
    }
    pthread_exit(NULL);
}

//check the threadpool is full;
int isThreadPoolFull(threadpool_t * pool)
{
    return pool->queueSize == pool->queueCapacity && !pool->shutdown;
}

//add task into threadpool
void addTask(threadpool_t *pool, void(*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while(isThreadPoolFull(pool))
    {
        //block the producer as the pool is full; 
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    if(pool->shutdown)
    {
       pthread_mutex_unlock(&pool->mutexPool);
       return;
    }
    // add task into threadpool;
    pool->taskQ[pool->queueRear].func  = func;
    pool->taskQ[pool->queueRear].arg   = arg;
    pool->queueRear = (pool->queueRear+1) % pool->queueCapacity; 
    pool->queueSize ++;
    //wake up consumer; 
    pthread_cond_signal(&pool->notEmpty);

    pthread_mutex_unlock(&pool->mutexPool);
}

//get busy pthread num;
int getBusyThreadNum(threadpool_t* pool)
{  
   pthread_mutex_lock(&pool->mutexBusy);
   int busyN = pool->busyNum;
   pthread_mutex_unlock(&pool->mutexBusy);
   return busyN;
}

//get alive pthrad num;
int getAliveThreadNum(threadpool_t* pool)
{
    pthread_mutex_lock(&pool->mutexPool);
   int aliveN = pool->aliveNum;
   pthread_mutex_unlock(&pool->mutexPool);
   return aliveN;
}

//destroy threadpool
int destroyThreadPool(threadpool_t* pool)
{
    if(pool == NULL)
    {
        return -1;
    }
    //shutdown pool;
    pool->shutdown = 1;
    //recycle manager thread.
    pthread_join(pool->managerID, NULL);

    //wakeup blocked thread;
    for(int i= 0; i< pool->aliveNum; ++i)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
     //free mem in heap;
     if(pool->taskQ)  free(pool->taskQ);
     if(pool->threadID) free(pool->threadID);

    //destory mutex lock;
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    //last step is free the threadpool 
    free(pool);
    pool = NULL;



    return 0;
}
