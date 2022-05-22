/*
 * @Author: Kyon0478 zhangcheng0478@163.com
 * @Date: 2022-05-10 19:51:50
 * @LastEditors: Kyon0478 zhangcheng0478@163.com
 * @LastEditTime: 2022-05-13 18:02:10
 * @FilePath: \200_Process_Thread\Thread_Trial\01_pthread\01_thread_create.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*describes the pthread basic usage, as 
    1. pthread_create(), 
    2.pthread_self() 
    3. sleep for thread suspend;
*/

void* printHello(void* args)
{
    printf("this is child thread %ld ! \n", pthread_self());
    // pthread_self() => type is pthread_t (unsigned long type)
    //                   which print out the thread ID;
}

int main()
{   
    pthread_t pth1; 
    pthread_create(&pth1, NULL, printHello, NULL);
    printf("this is main thread %ld ! \n", pthread_self());
    //method 1: let the main thread suspend for a while, then CPU time slice can give to child thread
    //sleep(3);  //wait for 3 second in Linux, and 3ms in Windows.

    //method2 : as main thread wait until child thread finished. 
     pthread_join(pth1, NULL);    
    return 0;
}
