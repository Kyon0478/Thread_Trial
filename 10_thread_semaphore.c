// compare to cond, it is more easier;
/*semaphore,
  the consumer would consider how many products can be used, which can be consumed by multiple threads simultaneously;
  the producer would consider the product container's buffer, which can be produced by multiple threads at the same time.


int sem_init(sem_t *sem, int pshared, unsigned int value);
// sem :     semaphore variable address;
// pshared : 0-> thread, !0 => process ;
// value  ：producer resource or consumer resource

int sem_destroy(sem_t *sem)

sem_t  1 for producer, 1 for consumer;

int sem_wait(sem_t *sem); //compared to => int pthread_cond_wait(pthread_cont_t *cond, pthread_mutex_t *lock);
    -> once called, the resource would get decreased ; if resource = 0; pthread is blocked;
int sem_post(sem_t *sem); //compared to => int pthread_cond_broadcast(pthread_cont_t *cond);
    -> once called, the resource would get increased ; if resource == upper. pthread is blocked;
int sem_getvalue(sem_t *sem, int *sval);

producer -> sem_wait  sval+1;
            sem_post  sval-1;

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

pthread_mutex_t    lock;  
pthread_rwlock_t   rwlock;
pthread_cond_t     condlock;

sem_t sem_p; // define semaphore for producer;
sem_t sem_c; // define semephore for consumer;

typedef struct Node
{
  int num;
  struct Node *next; 
} node_t;

node_t *head = NULL;

void* produceFun(void* args)
{
   while(1)
   { 
     //cannot add mutex-lock here, otherwise would get dead_lock
     sem_wait(&sem_p); // sem_p is produced to container upper limit;
     // critical area;
     pthread_mutex_lock(&lock);
     node_t *newnode = (node_t *)malloc(sizeof(node_t));
     newnode->num = rand() % 1000;
     newnode->next = head;
     head = newnode;   
     printf("Producer ID = %ld,  new node is %d \n", pthread_self(), newnode->num);
     pthread_mutex_unlock(&lock); // unlock then post;
     sem_post(&sem_c);
     sleep(1);
   }

   return NULL;
}

void* consumeFun(void* args)
{
    while(1)
    {
      sem_wait(&sem_c);
      pthread_mutex_lock(&lock);
      node_t *node = head; // pointer to the link head;
      printf("Consumer ID = %ld,  node value is %d\n", pthread_self(), node->num);
      head = head->next;
      free(node); // dont forget to free the HEAP;
      // node = NULL;

      pthread_mutex_unlock(&lock);
      sem_post(&sem_p);
      sleep(2);
    }
    return NULL;
}

int main()
{
  // inital semaphore for producer and consumer; 
  sem_init(&sem_p, 0, 5); // 0: for pshared in thread; //resource to 5
  sem_init(&sem_c, 0, 5); // resource up to 5;

  // if the resource is define as 1, there is no need to add mutex lock;
  // if sem_t resource >1 , multiple thread would race, need to add lock;
  pthread_mutex_init(&lock, NULL);
  
  //inital pthread
  pthread_t producer[5], consumer[5];
  for (int i = 0; i < 5;i++)
  {
     pthread_create(&producer[i], NULL, produceFun, NULL);
     pthread_create(&consumer[i], NULL, consumeFun, NULL);
  }
  
   ////thread block, resource recycle
   for (int i = 0; i < 5;i++)
   {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    sem_destroy(&sem_p);
    sem_destroy(&sem_c);

    return 0;
}
