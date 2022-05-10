/*
 * @Author: your name
 * @Date: 2022-05-10 20:11:33
 * @LastEditTime: 2022-05-10 21:28:48
 * @LastEditors: Kyon0478 zhangcheng0478@163.com
 * @Description: 鎵撳紑koroFileHeader鏌ョ湅閰嶇疆 杩涜璁剧疆: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \200_Process_Thread\Thread_Trial\02_pthread_pool\01_threadpool.h
 */
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//task struct 
typedef struct Task
{ 
    void(*func)(void *arg);  
    void* arg;
} task_t; 

//thread pool struct; 
typedef struct Threadpool
{ 
   //task queue;
   task_t* taskQ;
   int   queueCapacity;  //task capacity
   int   queueSize;      //current task quantity;
   int   queueFront;     //fetch data;
   int   queueRear;      //write data; 

   pthread_t  manageID;
   pthread_t  *threadID;
   int    minNum;    //minimum thread quantity;
   int    maxNum;    //maximum thread quantity;
   int    busyNum;   //working thread;
   int    aliveNum;  //alive thread;
   int    exitNum;   //to be killed thread;
   
   //task sync;
   pthread_mutex_t  mutexPool;   //MUTEX
   pthread_mutex_t  mutexBusy;   //lock for busyNum;
   pthread_cond_t   notFull;   //threadpool whether is full;
   pthread_cond_t   notEmpty;  //threadpool whether is empty;

   int  shutdown;   //whether destory the threadpool;  //destory =1, keep = 0;
} threadpool_t;
 
//initialize pthread pool
threadpool_t* threadPoolCreate(int queueSize, int minNum, int maxNum)
{
    //queueSize  => 线程池线程个数；
    //minNum / maxNum => 线程池最小/大线程数量;
    //create a pool on heap;
    threadpool_t* pool = (threadpool_t*)malloc(sizeof(threadpool_t));
    do
    {
        if(pool == NULL)
        {
            printf("malloc threadpool is failed .. \n");
            break;
        }
        
        pool->threadID = (pthread_t*)malloc(sizeof(pthread_t)* maxNum);
        if (pool->threadID == 0)
        {
            printf("malloc threadID is failed.. \n");
            break;
        }

        memset(pool->threadID, 0, sizeof(pthread_t)*maxNum);
        pool->maxNum = maxNum;
        pool->minNum = minNum;
        pool->busyNum = 0;
        pool->aliveNum = minNum;  //和最小个数相等；
        pool->exitNum  = 0 ;

        if(pthread_mutex_init(&pool->mutexPool, NULL) !=0 ||
        pthread_mutex_init(&pool->mutexBusy, NULL) !=0 ||
        pthread_cond_init(&pool->notEmpty,  NULL) !=0 ||
        pthread_cond_init(&pool->notFull,   NULL) !=0 )
        {
            printf("mutex init failed ... \n");
        } 

        //任务队列；
        pool->taskQ = (task_t*)malloc(sizeof(task_t) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear =0;

        pool->shutdown = 0; 

        //创建线程；
        pthread_create(&pool->manageID, NULL, manager, NULL);
        for(int i = 0; i< minNum; ++i)
        {  
            pthread_create(&pool->threadID[i], NULL, taskthread ,NULL)
        }
        //create pool successfully, return pool;
        return pool;
    } while (0);

     //release resource
     if (pool->threadID && pool)   free(pool->threadID);
     if (pool->taskQ && pool) free(pool->taskQ);
     if (pool)  free(pool);

    return NULL;
}

//destory pthread pool


//add task into pthread pool


//get busy pthread num;

//get alive pthrad num;


#endif  //_PTHREADPOOL_H



