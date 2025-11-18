#include "./Simulation.h"
#include "../Queue/Queue.h"
#include "../Scheduler/Scheduler.h"
#include "../Simulation/SimulationLogger.h"
#include "../Job/Job.h"
#include "../IO/IO.h"
#include <stdio.h>
#include <stdlib.h>
#define BLUE "\x1B[34m"
#define RED "\x1B[31m"
#define WHITE "\x1B[37m"

Simulation *newSimulation(char *file_name, ScheduleType schedule_type)
{
  // Get the number of jobs
  // int number_of_jobs = getNumberofJobs(file_name);
  // Parse the input file
  JobQueue *job_arrival_q = getArrivalQueueFromFile(file_name);

  if (!job_arrival_q)
  {
    printf("JobQueue was Null");
    exit(1);
  }
  Simulation *simulation = malloc(sizeof(Simulation));
  if (!simulation)
  {
    printf("Malloc Failed");
    exit(1);
  }
  Scheduler *scheduler = newScheduler(schedule_type);
  if (!scheduler)
  {
    printf("Failed to create Scheduler");
  }
  simulation->total_jobs = job_arrival_q->size;
  simulation->getArrivingJobs = getArrivingJobs;
  simulation->scheduler = scheduler;
  simulation->arrivalQueue = job_arrival_q;
  simulation->done = 0;
  simulation->completed = newJobQueue();
  simulation->n_completed_jobs = 0;
  return simulation;
}

JobQueue *getArrivingJobs(Simulation *self, int time)
{
  JobQueue *q = newJobQueue();
  JobQueue *arrivalq = self->arrivalQueue;
  int count = 0;
  while (!isEmpty(arrivalq) && peek(arrivalq)->arrivalTime == time)
  {
    Job *job = dequeue(arrivalq);
    enqueue(q, job);
    count++;
  }
  return q;
}

int checkIOCompletion(Simulation *simulation, Scheduler *scheduler, int clock)
{

  if (!isEmpty(scheduler->waiting_for_IO_queue))
  {
    JobQueue *new_queue = newJobQueue();
    while (!isEmpty(scheduler->waiting_for_IO_queue))
    {
      Job *job = dequeue(scheduler->waiting_for_IO_queue);
      int status = IO_complete();
      if (status == 1)
      {
        logJobCompletion(simulation, job, clock);
        job->status = READY;
        scheduler->addJob(scheduler, job);
      }
      else
      {
        job->timeWaitingForIO++;
        enqueue(new_queue, job);
      }
    }
    scheduler->waiting_for_IO_queue = new_queue;
    return 1;
  }
  return 0;
}

int isComplete(Simulation *simulation)
{
  return simulation->n_completed_jobs == simulation->total_jobs;
}

int add_ready_to_run_job(Simulation *s, int clock)
{
  JobQueue *arrivingJobs = s->getArrivingJobs(s, clock);
  if (arrivingJobs == NULL)
  {
    return -1;
  }
  for (int i = 0; i < arrivingJobs->size - 1; i++)
  {
    for (int j = 0; j < arrivingJobs->size - i - 1; j++)
    {
      if (arrivingJobs->jobs[j]->pid > arrivingJobs->jobs[j + 1]->pid)
      {
        Job *temp = arrivingJobs->jobs[j];
        arrivingJobs->jobs[j] = arrivingJobs->jobs[j + 1];
        arrivingJobs->jobs[j + 1] = temp;
      }
    }
  }
  while (!isEmpty(arrivingJobs))
  {
    Job *j = dequeue(arrivingJobs);
    if (j != NULL)
    {
      logJobArrival(s, j, clock);
    }
    s->scheduler->addJob(s->scheduler, j);
  }
  return 0;
}