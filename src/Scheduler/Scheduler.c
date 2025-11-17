#include "Scheduler.h"
#include "../Queue/Queue.h"
#include "../Queue/MinHeap.h"
#include "stdlib.h"
#include "stdio.h"

#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"
#define KGRN  "\x1B[32m"


Scheduler* newScheduler(ScheduleType schedule_type)
{
    Scheduler *scheduler = malloc(sizeof(Scheduler));
    scheduler->scheduleType = schedule_type;
    scheduler->n_completed_jobs = 0;
    switch (schedule_type)
    {
    case RR:
        scheduler->getNextJob = RR_getNext;
        scheduler->addJob = RR_addJob;
        scheduler->Levels = newJobQueues(1); // Default is 1
        scheduler->waiting_for_IO_queue = newJobQueue();
        scheduler->quantum[0] = 5;
        scheduler->increment_ready_to_run_state = increment_ready_to_run_state_RR;
        scheduler->preemptive = NULL;
        break;
    case SJF:
        scheduler->getNextJob = SJF_getNext;
        scheduler->addJob = SJF_addJob;
        scheduler->sjf_heap = newJobMinHeap(20);
        scheduler->preemptive = SJF_preemptive;
        scheduler->increment_ready_to_run_state = increment_ready_to_run_state_SJF;
        break;
    case MLFS:
        scheduler->getNextJob = MLFS_getNext;
        scheduler->addJob = MLFS_addJob;
        scheduler->Levels = newJobQueues(3); // 3 levels for MLFS
        scheduler->quantum[0] = 1;
        scheduler->quantum[1] = 2;
        scheduler->quantum[2] = 3;
        scheduler->current_quantum = scheduler->quantum[0];
        scheduler->preemptive = MLFS_preemptive;
        scheduler->increment_ready_to_run_state = increment_ready_to_run_state_MLFS;
        break;
    default:
        scheduler->getNextJob = RR_getNext;
        scheduler->addJob = RR_addJob;
        break;
    }
    scheduler->waiting_for_IO_queue = newJobQueue();
    return scheduler;
}
int isJobPreempted(Scheduler* scheduler, Job* job){
    if (job == NULL){
        return 1;
    }
    if(scheduler->preemptive ==NULL){
        return 0;
    }
    return scheduler->preemptive(scheduler, job);

}

int increment_ready_to_run_state_RR(Scheduler* self){
    JobQueue* queue = ((Scheduler *)self)->Levels[0];
    for (int i = 0; i < queue->size; i++) {
        queue->jobs[(queue->front + i) % queue->capacity]->timeInReadyToRunState++;
    }
    return 0;
}

int increment_ready_to_run_state_MLFS(Scheduler* self){
    for (int i = 0; i < MAX_LEVELS; i++) {
        JobQueue* queue = ((Scheduler *)self)->Levels[i];
        for (int j = 0; j < queue->size; j++) {
            queue->jobs[(queue->front + j) % queue->capacity]->timeInReadyToRunState++;
        }
    }
    return 0;
}

int increment_ready_to_run_state_SJF(Scheduler* self){
    JobMinHeap* heap = ((Scheduler *)self)->sjf_heap;
    for (int i = 0; i < heap->size; i++) {
        heap->jobs[i]->timeInReadyToRunState++;
    }
    return 0;
}

int SJF_preemptive(Scheduler* self, Job *j)
{

    // printf("Checking SJF preemption for Job PID: %d with time remaining: %d\n", j->pid, j->timeRemaining);
    if (((Scheduler *)self)->sjf_heap->size == 0)
    {
        return 0;
    }

    Job *next_job = peek_MinHeap(((Scheduler *)self)->sjf_heap);
    if (next_job == NULL)
    {
        return 0;
    }
    if (next_job->duration < j->duration)
    {
        printf("\t%s[Preempting] Job PID: %d with Job PID: %d\n%s", KRED, j->pid, next_job->pid, KWHT);
        return 1;
    }
    return 0;
}

int MLFS_preemptive(Scheduler* self, Job *j)
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

Job *RR_getNext(Scheduler* self)
{
    int PRIORITY = 0;

    if (isEmpty(((Scheduler *)self)->Levels[PRIORITY]))
    {
        return NULL;
    }
    Job* j =  dequeue(((Scheduler *)self)->Levels[PRIORITY]);
    j->timeSlice = ((Scheduler *)self)->quantum[PRIORITY];
    ((Scheduler *)self)->current_quantum = j->timeSlice;
    j->priority = PRIORITY;
    j->status = RUNNING;
    return j;
}
int RR_addJob(Scheduler* self, Job *j)
{
    int PRIORITY = 0;
    j->priority = PRIORITY;
    j->status = READY;
    j->timeSlice = ((Scheduler *)self)->quantum[PRIORITY];
    int err = enqueue(((Scheduler *)self)->Levels[PRIORITY], j);
    if (err)
    {
        printf("Enqueue Failed");
    }
    return 0;
}

Job *SJF_getNext(Scheduler* self)
{
    return extractMinJobMinHeap(((Scheduler *)self)->sjf_heap);
}

int SJF_addJob(Scheduler* self, Job *j)
{
    insertJobMinHeap(((Scheduler *)self)->sjf_heap, j);
    return 0;
}

Job *MLFS_getNext(Scheduler* self)
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
    if (job == NULL)
    {
        return NULL;
    }
    job->timeSlice = ((Scheduler *)self)->quantum[priority];
    job->priority = priority;
    job->status = RUNNING;
    ((Scheduler *)self)->current_quantum = ((Scheduler *)self)->quantum[priority];
    return job;
}

int MLFS_addJob(Scheduler* self, Job *j)
{
    int PRIORITY = 0;

    if (j->status == RUNNING) {
        PRIORITY = j->priority;
        if (PRIORITY < MAX_LEVELS - 1) {
            PRIORITY += 1;
        }
    }
    else {
        PRIORITY = 0;
    }
    int err = enqueue(((Scheduler *)self)->Levels[PRIORITY], j);
    if (err)
    {
        printf("Enqueue Failed");
    }
    return 0;
}
