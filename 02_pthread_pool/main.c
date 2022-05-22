/*
 * @Author: your name
 * @Date: 2022-05-10 20:11:40
 * @LastEditTime: 2022-05-22 13:49:36
 * @LastEditors: Kyon0478 zhangcheng0478@163.com
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \200_Process_Thread\main.c
 */

#include <stdio.h>
#include "01_threadpool.h"

void taskFunc(void* arg)
{   
    int num = *(int*)arg;
    printf("thread %ld is working, number is %d , \n", pthread_self(), num);
    usleep(1000);
}

int main()
{  
   //create threadpool
   //threadpool_t* threadPoolCreate(int qsize, int minN, int maxN);
   threadpool_t* pool = threadPoolCreate(100, 3, 10);

   for (int i = 0 ; i< 100 ; i++)
   {
       //void addTask(Threadpool_t *pool, void(*func)(void*), void* arg);
       int *num = (int* )malloc(sizeof(int));
       *num  = i + 100;
       addTask(pool, taskFunc, num);
   }
   sleep(5);
   destroyThreadPool(pool);

   return 0;

}