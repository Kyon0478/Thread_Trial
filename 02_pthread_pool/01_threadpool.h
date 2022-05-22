/*
 * @Author: your name
 * @Date: 2022-05-10 20:11:33
 * @LastEditTime: 2022-05-22 13:59:17
 * @LastEditors: Kyon0478 zhangcheng0478@163.com
 * @Description: 闁瑰灚鎸哥槐鎱縪roFileHeader闁哄被鍎冲﹢鍛存煀瀹ュ洨鏋� 閺夆晜绋栭、鎴犳媼閸撗呮瀭: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \200_Process_Thread\Thread_Trial\02_pthread_pool\01_threadpool.h
 */
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

//task struct 
typedef struct Task
{ 
    void(*func)(void *arg); //func pointer   
    void* arg;              //variables for the func;
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

   pthread_t   managerID;
   pthread_t  *threadID;     //worked thread. 
   int    minNum;    //minimum thread quantity;
   int    maxNum;    //maximum thread quantity;
   int    busyNum;   //working thread;
   int    aliveNum;  //alive thread;
   int    exitNum;   //to be killed thread;
   
   //task sync;
   pthread_mutex_t  mutexPool;   //MUTEX for the whole thread pool;
   pthread_mutex_t  mutexBusy;   //lock for busyNum;
   pthread_cond_t   notFull;   //threadpool whether is full;
   pthread_cond_t   notEmpty;  //threadpool whether is empty;

   int  shutdown;   //whether destory the threadpool;  //destory =1, keep = 0;
} threadpool_t;

threadpool_t* threadPoolCreate(int qsize, int minN, int maxN);

void* worker(void* args);

void* manager(void* args);

//add task into pthreadpool
void addTask(threadpool_t *pool, void(*func)(void*), void* arg);

int isThreadPoolFull(threadpool_t * pool);

void threadExit(threadpool_t * pool);

int getBusyThreadNum(threadpool_t* pool);

int getAliveThreadNum(threadpool_t* pool);

//destroy threadpool
int destroyThreadPool(threadpool_t* pool);

#endif  //_PTHREADPOOL_H



