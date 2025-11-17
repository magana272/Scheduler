#pragma once
#include "../Job/Job.h"

typedef struct JobMinHeap
{
    int capacity;
    int size;
    struct Job **jobs;
} JobMinHeap;

JobMinHeap *newJobMinHeap(int capacity);
Job *peek_MinHeap(JobMinHeap *heap);
int insertJobMinHeap(JobMinHeap *heap, Job *job);
Job *extractMinJobMinHeap(JobMinHeap *heap);
void freeJobMinHeap(JobMinHeap *heap);
