#include <stdio.h>
#include <stdlib.h>
#include "../src/Queue/MinHeap.h"

int main() {
    JobMinHeap* heap = newJobMinHeap(10);
    Job* job1 = malloc(sizeof(Job));
    job1->pid = 1;
    job1->duration = 5;

    Job* job2 = malloc(sizeof(Job));
    job2->pid = 2;
    job2->duration = 3;

    Job* job3 = malloc(sizeof(Job));
    job3->pid = 3;
    job3->duration = 8;

    insertJobMinHeap(heap, job1);
    insertJobMinHeap(heap, job2);
    insertJobMinHeap(heap, job3);

    while (heap->size > 0) {
        Job* minJob = extractMinJobMinHeap(heap);
        printf("Extracted Job PID: %d, Duration: %d\n", minJob->pid, minJob->duration);
    }
    freeJobMinHeap(heap);
    free(job1);
    free(job2);
    free(job3);

    return 0;
}

