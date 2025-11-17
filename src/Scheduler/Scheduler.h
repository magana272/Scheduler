# pragma once
#include "../Queue/Queue.h"
#include "../Job/Job.h"
#include "../Queue/MinHeap.h"
#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"
#define KGRN  "\x1B[32m"
#define MAX_LEVELS 3

typedef enum {RR, SJF, MLFS} ScheduleType;

typedef struct Scheduler Scheduler;
typedef struct Scheduler{
  int quantum[MAX_LEVELS]; // Time slices for each level
  int n_completed_jobs; // Number of completed jobs
  int current_quantum; // Current quantum for the running job
  ScheduleType scheduleType; // Scheduling algorithm type
  JobQueue** Levels; // Array of job queues for different levels
  JobMinHeap* sjf_heap; // Min-heap for SJF scheduling

  JobQueue* waiting_for_IO_queue; // Queue for jobs waiting for I/O
  Job* (*getNextJob)(Scheduler* s); // Function pointer to get the next job
  int (*addJob)(Scheduler* s, Job* j); // Function pointer to add a job
  int (*preemptive)(Scheduler* s, Job* j); // Function pointer to check for preemption
  int (*increment_ready_to_run_state)(Scheduler* self);
} Scheduler;

Scheduler* newScheduler(ScheduleType schedule_type);
Job* NextJob(Scheduler* Scheduler);

Job* get_shortest_job(JobQueue** queues);
// SJF preemptive
int SJF_preemptive(Scheduler* self, Job* j);
Job* SJF_getNext(Scheduler* self);
int SJF_addJob(Scheduler* self, Job* j);

// Round Robin
Job* RR_getNext(Scheduler* self);
int RR_addJob(Scheduler* self, Job* j);

// Round Robin
Job* MLFS_getNext(Scheduler* self);
int MLFS_addJob(Scheduler* self, Job* j);
int MLFS_preemptive(Scheduler* self, Job* j);

int increment_ready_to_run_state_RR(Scheduler* self);
int increment_ready_to_run_state_MLFS(Scheduler* self);
int increment_ready_to_run_state_SJF(Scheduler* self);

int isJobPreempted(Scheduler* scheduler, Job* job);