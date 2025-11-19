#include <stdio.h>
#include <stdlib.h>
#include "MinHeap.h"
// Min-Heap implementation for Jobs based on their duration (for SJF scheduling)
JobMinHeap *newJobMinHeap(int capacity)
{
    JobMinHeap *heap = malloc(sizeof(JobMinHeap));
    if (!heap)
    {
        printf("Malloc Failed");
        exit(1);
    }
    heap->capacity = capacity;
    heap->size = 0;
    heap->jobs = malloc(sizeof(Job *) * capacity);
    if (!heap->jobs)
    {
        printf("Malloc Failed");
        exit(1);
    }
    return heap;
}
int insertJobMinHeap(JobMinHeap *heap, Job *job)
{
    if (heap->size == heap->capacity)
    {
        printf("Heap is full");
        return -1;
    }
    heap->jobs[heap->size] = job;
    heap->size++;
    int index = heap->size - 1;
    while (index != 0)
    {
        int parentIndex = (index - 1) / 2;
        if (heap->jobs[parentIndex]->timeRemaining < heap->jobs[index]->timeRemaining)
        {
            break;
        }
        if (heap->jobs[parentIndex]->timeRemaining == heap->jobs[index]->timeRemaining &&
            heap->jobs[parentIndex]->pid <= heap->jobs[index]->pid)
        {
            break;
        }
        Job *temp = heap->jobs[parentIndex];
        heap->jobs[parentIndex] = heap->jobs[index];
        heap->jobs[index] = temp;
        index = parentIndex;
    }
    return 0;
}
Job *extractMinJobMinHeap(JobMinHeap *heap)
{
    if (heap->size == 0)
    {
        return NULL;
    }
    Job *minJob = heap->jobs[0];
    heap->jobs[0] = heap->jobs[heap->size - 1];
    heap->size--;
    int index = 0;
    while (1)
    {
        int leftChildIndex = 2 * index + 1;
        int rightChildIndex = 2 * index + 2;
        int smallestIndex = index;

        if (leftChildIndex < heap->size && heap->jobs[leftChildIndex]->timeRemaining < heap->jobs[smallestIndex]->timeRemaining)
        {
            smallestIndex = leftChildIndex;
        }
        if (rightChildIndex < heap->size && heap->jobs[rightChildIndex]->timeRemaining < heap->jobs[smallestIndex]->timeRemaining)
        {
            smallestIndex = rightChildIndex;
        }
        if (leftChildIndex < heap->size && heap->jobs[leftChildIndex]->timeRemaining == heap->jobs[smallestIndex]->timeRemaining)
        {
            if (heap->jobs[leftChildIndex]->pid < heap->jobs[smallestIndex]->pid)
            {
                smallestIndex = leftChildIndex;
            }
        }
        if (rightChildIndex < heap->size && heap->jobs[rightChildIndex]->timeRemaining == heap->jobs[smallestIndex]->timeRemaining)
        {
            if (heap->jobs[rightChildIndex]->pid < heap->jobs[smallestIndex]->pid)
            {
                smallestIndex = rightChildIndex;
            }
        }
        if (smallestIndex == index)
        {
            break;
        }
        Job *temp = heap->jobs[index];
        heap->jobs[index] = heap->jobs[smallestIndex];
        heap->jobs[smallestIndex] = temp;
        index = smallestIndex;
    }
    return minJob;
}
void freeJobMinHeap(JobMinHeap *heap)
{
    if (heap)
    {
        free(heap->jobs);
        free(heap);
    }
}

Job *peek_MinHeap(JobMinHeap *heap)
{
    if (heap->size == 0)
    {
        return NULL;
    }
    return heap->jobs[0];
}
