#include "Scheduler.h"
#include "../Queue/Queue.h"
#include "../Queue/MinHeap.h"
#include "stdlib.h"
#include "stdio.h"

Scheduler *newScheduler(ScheduleType schedule_type)
{
    Scheduler *scheduler = malloc(sizeof(scheduler));
    scheduler->scheduleType = schedule_type;
    scheduler->n_completed_jobs = 0;
    switch (schedule_type)
    {
    case RR:
        scheduler->getNextJob = RR_getNext;
        scheduler->addJob = RR_addJob;
        scheduler->Levels = newJobQueues(1); // Default is 1
        break;
    case SJF:
        scheduler->getNextJob = SJF_getNext;
        scheduler->addJob = SJF_addJob;
        scheduler->sjf_heap = newJobMinHeap(20);
        scheduler->preemptive = SJF_preemptive;
        break;
    case MLFS:
        scheduler->getNextJob = MLFS_getNext;
        scheduler->addJob = MLFS_addJob;
        scheduler->Levels = newJobQueues(3); // 3 levels for MLFS
        scheduler->quantum[0] = 4;
        scheduler->quantum[1] = 8;
        scheduler->quantum[2] = 12;
        scheduler->current_quantum = scheduler->quantum[0];
        scheduler->preemptive = MLFS_preemptive;
        break;
    default:
        scheduler->getNextJob = RR_getNext;
        scheduler->addJob = RR_addJob;
        break;
    }
    scheduler->waiting_for_IO_queue = newJobQueue();
    return scheduler;
}

int SJF_preemptive(void *self, Job *j)
{
    if (((Scheduler *)self)->sjf_heap->size == 0)
    {
        return 0;
    }
    Job *next_job = ((Scheduler *)self)->sjf_heap->jobs[0];
    if (next_job->duration < j->timeRemaining)
    {
        return 1;
    }
    return 0;
}

int MLFS_preemptive(void *self, Job *j)
{
    for (int level = 0; level < j->priority; level++)
    {
        if (!isEmpty(((Scheduler *)self)->Levels[level]))
        {
            return 1;
        }
    }
    return 0;
}

Job *RR_getNext(void *self)
{
    int PRIORITY = 0;
    if (isEmpty(((Scheduler *)self)->Levels[PRIORITY]))
    {
        return NULL;
    }
    return dequeue(((Scheduler *)self)->Levels[PRIORITY]);
}
int RR_addJob(void *self, Job *j)
{
    int PRIORITY = 0;
    int err = enqueue(((Scheduler *)self)->Levels[PRIORITY], j);
    if (err)
    {
        printf("Enqueue Failed");
    }
    return 0;
}

Job *SJF_getNext(void *self)
{
    return extractMinJobMinHeap(((Scheduler *)self)->sjf_heap);
}

int SJF_addJob(void *self, Job *j)
{
    insertJobMinHeap(((Scheduler *)self)->sjf_heap, j);
    return 0;
}

Job *MLFS_getNext(void *self)
{
    int priority = 0;
    while (priority < MAX_LEVELS)
    {
        if (!isEmpty(((Scheduler *)self)->Levels[priority]))
        {
            break;
        }
        priority++;
    }
    Job *job = dequeue(((Scheduler *)self)->Levels[priority]);
    ((Scheduler *)self)->current_quantum = ((Scheduler *)self)->quantum[priority];
    return job;
}

int MLFS_addJob(void *self, Job *j)
{
    int PRIORITY = 0;
    while (PRIORITY < MAX_LEVELS - 1 && !isEmpty(((Scheduler *)self)->Levels[PRIORITY]))
    {
        PRIORITY++;
    }
    int err = enqueue(((Scheduler *)self)->Levels[PRIORITY], j);
    if (err)
    {
        printf("Enqueue Failed");
    }
    return 0;
}
