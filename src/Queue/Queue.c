#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Job/Job.h"
#include "../Scheduler/Scheduler.h"


Job* peek(JobQueue* q){
    if(isEmpty(q)){
        return NULL;
    }
    return q->jobs[q->front];
}

int isEmpty(JobQueue* q){
    return q->size == 0;
}
int isFull(JobQueue* q){
    return q->size == q->capacity;
}
int enqueue(JobQueue* q, Job* j){
    if(isFull(q)){
        return -1; // Queue is full
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->jobs[q->rear] = j;
    q->size++;
    return 0; // Success
}
Job* dequeue(JobQueue* q){
    if(isEmpty(q)){
        return NULL; // Queue is empty
    }
    Job* job = q->jobs[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return job;
}
JobQueue* newJobQueue(){
    JobQueue* q = malloc(sizeof(JobQueue));
    if(!q){
        printf("Malloc Failed");
        exit(1);
    }
    q->capacity = 100;
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->jobs = malloc(sizeof(Job*) * q->capacity);
    if(!q->jobs){
        printf("Malloc Failed");
        exit(1);
    }
    return q;
}
JobQueue** newJobQueues(int levels){
    JobQueue** queues = malloc(sizeof(JobQueue*) * levels);
    if(!queues){
        printf("Malloc Failed");
        exit(1);
    }
    for(int i = 0; i < levels; i++){
        queues[i] = newJobQueue();
    }
    return queues;
}