#include <stdio.h>
#include <stdlib.h>
#include "../src/Scheduler/Scheduler.h"

int testSJFScheduler();
int testRRScheduler();

int main()
{
  testSJFScheduler();
  testRRScheduler();
  return 0;
}
int testRRScheduler()
{
  Scheduler *scheduler = newScheduler(RR);
  printf("Scheduler created with scheduleType: %d\n", scheduler->scheduleType);
  if (scheduler->scheduleType != RR)
  {
    printf("Test Failed: Scheduler type mismatch\n");
    return 1;
  }
  Job *job1 = malloc(sizeof(Job));
  job1->pid = 1;
  job1->duration = 5;
  Job *job2 = malloc(sizeof(Job));
  job2->pid = 2;
  job2->duration = 3;
  Job *job3 = malloc(sizeof(Job));
  job3->pid = 3;
  job3->duration = 8;
  scheduler->addJob(scheduler, job1);
  scheduler->addJob(scheduler, job2);
  scheduler->addJob(scheduler, job3);
  Job *nextJob = scheduler->getNextJob(scheduler);
  if (nextJob->pid != 1)
  {
    printf("Test Failed: Expected job1, got job with PID %d\n", nextJob->pid);
    return 1;
  }
  nextJob = scheduler->getNextJob(scheduler);
  if (nextJob->pid != 2)
  {
    printf("Test Failed: Expected job2, got job with PID %d\n", nextJob->pid);
    return 1;
  }
  nextJob = scheduler->getNextJob(scheduler);
  if (nextJob->pid != 3)
  {
    printf("Test Failed: Expected job3, got job with PID %d\n", nextJob->pid);
    return 1;
  }
  printf("All tests passed!\n");
  return 0;
}

int testSJFScheduler()
{
  Scheduler *scheduler = newScheduler(SJF);
  printf("Scheduler created with scheduleType: %d\n", scheduler->scheduleType);
  if (scheduler->scheduleType != SJF)
  {
    printf("Test Failed: Scheduler type mismatch\n");
    return 1;
  }
  Job *job1 = malloc(sizeof(Job));
  job1->pid = 1;
  job1->duration = 5;
  Job *job2 = malloc(sizeof(Job));
  job2->pid = 2;
  job2->duration = 3;
  Job *job3 = malloc(sizeof(Job));
  job3->pid = 3;
  job3->duration = 8;
  scheduler->addJob(scheduler, job1);
  scheduler->addJob(scheduler, job2);
  scheduler->addJob(scheduler, job3);
  scheduler->getNextJob(scheduler); // Should be job2

  if (scheduler->sjf_heap->size != 2)
  {
    printf("Test Failed: Heap size mismatch after one extraction\n");
    return 1;
  }
  Job *nextJob = scheduler->getNextJob(scheduler); // Should be job1
  if (nextJob->pid != 1)
  {
    printf("Test Failed: Expected job1, got job with PID %d\n", nextJob->pid);
    return 1;
  }
  printf("All tests passed!\n");
  return 0;
}
