#include<stdio.h>
#include<stdlib.h>

#include"queue.h"


void * topqueue(QueueC * queue){
    Node * temp;
    sem_wait(&(queue->sema));
    if(queue->capacity>0){
        temp=  queue->head->data;
    }
    sem_post(&(queue->sema));
    return temp;

}



int enqueue(QueueC* queue,Node* node)
{
    if(queue->capacity==queue->counter)
        return 1;
       //free(node);
    else
        {
            printf("enque!!!!!!!!!!1\n");
            sem_wait(&queue->sema);
            if(queue->head==NULL)
            {
                queue->tail=queue->head=node;
                queue->counter++;
            }
            else{

            queue->tail->next=node;
            queue->counter++;
            }
            sem_post(&queue->sema);
            return 0;
        }
}

Node* dequeue(QueueC* queue)
{
    if(queue->counter==0)
        return NULL;
    sem_wait(&queue->sema);
    queue->counter--;
    Node *n=queue->head;

//  int** base_mat=(int**)(n->data);
//    for(int i=0;i<240;i++){
//        for(int j=0;j<320;j++){
//            printf("gfhbjnkm %d\n",base_mat[i][j]);
//        }
//    }


    queue->head=queue->head->next;
    sem_post(&queue->sema);
    return n;
}



Node* createNode(void* data)
{
    Node* node=(Node*)calloc(1,sizeof(Node));
    if(!node)exit(1);
    node->data=data;
    node->next=NULL;
    return node;

}



void freeNode(Node* node)
{
    if(!node)
        free(node);
}

QueueC* createQueue(int max)
{
    QueueC* queue=(QueueC*)malloc(sizeof(QueueC));
    if(!queue)exit(1);
    queue->capacity=max;
    queue->counter=0;
    queue->head=NULL;
    queue->tail=NULL;
    sem_init(&queue->sema,0,1);
    return queue;
}
void freeQueue(QueueC* queue)
{
    while(queue->head)
        freeNode(dequeue(queue));
    free(queue);
}
int isEmpty(QueueC* queue){
    if(queue->counter>0){
        return 0;
    }
    return 1;
    //return !(queue->counter);
}



