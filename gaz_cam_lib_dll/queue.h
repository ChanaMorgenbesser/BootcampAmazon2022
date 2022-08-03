#ifndef QUEUE_H
#define QUEUE_H
#include<semaphore.h>

typedef struct node{
    void* data;
    struct node* next;
}Node;
typedef struct queue{
    Node*head;
    Node*tail;
    int capacity;
    int counter;
    sem_t sema;
}QueueC,*p_queue;

int enqueue(QueueC* queue,Node* node);
Node* dequeue(QueueC* queue);
Node* createNode(void* data);
void * topqueue(QueueC * queue);
void freeNode(Node* node);
void freeQueue(QueueC* queue);
QueueC*createQueue(int max);
int isEmpty(QueueC* queue);
#endif // QUEUE_H
