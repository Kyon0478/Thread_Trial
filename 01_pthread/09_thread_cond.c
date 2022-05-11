#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX 50

// normally condition is cooperate with mutex ;
pthread_mutex_t  lock;
pthread_cond_t cond;  // for consumer limit; 
pthread_cond_t cond2; // for producer upper limit;

//create link
typedef struct Node 
{
    int           num;
    struct Node  *next;
} node_t;
//create head link
node_t *head = NULL;

static int node_num = 0;

void* produceFun(void* args)
{
    // as produce container is link, there is no upper limit;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(node_num > 10)
        {
            // block the consume thread;
            // !!!Important
            //if the mutex lock the critical area, but if the pthread_cond_wait check the condition is not fulfilled, it would unlock the critical area;
            pthread_cond_wait(&cond2, &lock);
        }
        // critical zone; 
        node_t *newnode = (node_t *)malloc(sizeof(node_t));
        newnode->num = rand() % 1000;
        newnode->next = head;
        head = newnode;   //key step.
        node_num++; 
        printf("Producer ID = %ld,  new node is %d , node_num = %d\n", pthread_self(), newnode->num, node_num);
        pthread_mutex_unlock(&lock);

        // wake up consumer;
        pthread_cond_broadcast(&cond);
        sleep(1);
    }
    return NULL;
}

void* consumeFun(void* args)
{
    while(1)
    {   
        pthread_mutex_lock(&lock);
        while (head == NULL) // need wake up by producer thread;
        {
            // block the consume thread;
            pthread_cond_wait(&cond, &lock);
        }
        node_t *node = head; // pointer to the link head;
        node_num--;
        printf("Consumer ID = %ld,  node value is %d, node_num = %d\n", pthread_self(), node->num, node_num);
        head = head->next;
        free(node); // dont forget to free the HEAP;
        // node = NULL;
        pthread_mutex_unlock(&lock);

        pthread_cond_broadcast(&cond2);
        sleep(2);
    }
    return NULL;
}

int main()
{
    // init condi and mutex;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond2, NULL);

    // create producer and consumer;
    pthread_t producer[5], consumer[5];
    for (int i = 0; i < 5; i++)
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

    // release condi and mutex;
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond2);
    return 0;
}