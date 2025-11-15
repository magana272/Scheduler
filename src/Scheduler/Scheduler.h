# pragma once
#include "../Queue/Queue.h"
#include "../Job/Job.h"
#include "../Queue/MinHeap.h"

#define MAX_LEVELS 3

typedef enum {RR, SJF, MLFS} ScheduleType;


typedef struct Scheduler{
  int quantum[MAX_LEVELS]; // Time slices for each level
  int n_completed_jobs; // Number of completed jobs
  int current_quantum; // Current quantum for the running job
  ScheduleType scheduleType; // Scheduling algorithm type
  JobQueue** Levels; // Array of job queues for different levels


  JobMinHeap* sjf_heap; // Min-heap for SJF scheduling

  JobQueue* waiting_for_IO_queue; // Queue for jobs waiting for I/O
  Job* (*getNextJob)(void* s); // Function pointer to get the next job
  int (*addJob)(void* s, Job* j); // Function pointer to add a job
  int (*preemptive)(void* s, Job* j); // Function pointer to check for preemption

} Scheduler;

Scheduler* newScheduler(ScheduleType schedule_type);
Job* NextJob(Scheduler* Scheduler);

Job* get_shortest_job(JobQueue** queues);

int SJF_preemptive(void* self, Job* j);
Job* SJF_getNext(void* self);
int SJF_addJob(void* self, Job* j);


Job* RR_getNext(void* self);
int RR_addJob(void* self, Job* j);

Job* MLFS_getNext(void* self);
int MLFS_addJob(void* self, Job* j);
int MLFS_preemptive(void* self, Job* j);

