#pragma once
#include "../Job/Job.h"

typedef struct JobQueue
{
    int front;
    int rear;
    int capacity;
    int size;
    struct Job **jobs;
} JobQueue;

int isEmpty(JobQueue *q);
int isFull(JobQueue *q);
int enqueue(JobQueue *q, Job *j);
Job *dequeue(JobQueue *q);
Job *peek(JobQueue *q);
JobQueue *newJobQueue();
JobQueue **newJobQueues(int levels);
JobQueue *getArrivalQueueFromFile(char *file_name);
void printQueue(JobQueue *q);