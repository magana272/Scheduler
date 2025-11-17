#include "./Simulation.h"
#include "../Queue/Queue.h"
#include "../Scheduler/Scheduler.h"
#include "../Job/Job.h"
#include "../IO/IO.h"
#include <stdio.h>
#include <stdlib.h>

JobQueue *readInput(char *file_name)
{
  /**********
   * Read the input file and create a JobQueue
   * 123:0:10:1 Process Id: Arrival time : Service time : Priority
   * 124:1:20:0
   * *********/
  FILE *file = fopen((const char *)file_name, "r");
  if (!file)
  {
    printf("Failed to open file %s\n", file_name);
    return NULL;
  }
  JobQueue *job_arrival_q = newJobQueue();
  char line[400];

  while (fgets(line, sizeof(line), file))
  {
    Job *job = parseJob(line);
    if (job)
    {
      enqueue(job_arrival_q, job);
      for (int i = job_arrival_q->size - 1; i > 0; i--)
      {
        if (job_arrival_q->jobs[i]->arrivalTime < job_arrival_q->jobs[i - 1]->arrivalTime)
        {
          Job *temp = job_arrival_q->jobs[i];
          job_arrival_q->jobs[i] = job_arrival_q->jobs[i - 1];
          job_arrival_q->jobs[i - 1] = temp;
        }
        else
        {
          break;
        }
      }
    }
  }
  fclose(file);
  return job_arrival_q;
}
int getNumberofJobs(char *file_name)
{
  FILE *file = fopen(file_name, "r");
  if (!file)
  {
    printf("Failed to open file %s\n", file_name);
    return -1;
  }
  int count = 0;
  char line[400];
  while (fgets(line, sizeof(line), file))
  {
    count++;
  }
  fclose(file);
  return count;
}

Job *parseJob(char *line)
{
  Job *job = malloc(sizeof(Job));
  if (!job)
  {
    printf("Malloc Failed");
    exit(1);
  }
  sscanf(line, "%d:%d:%d:%d", &job->pid, &job->arrivalTime, &job->duration, &job->priority);
  job->timeRemaining = job->duration;
  job->status = READY;
  job->timeSlice = 0;
  job->timeInReadyToRunState = 0;
  job->timeWaitingForIO = 0;
  job->timeInRunningState = 0;
  job->completionTime = 0;
  return job;
}

Simulation *newSimulation(char *file_name, ScheduleType schedule_type)
{
  // Get the number of jobs
  int number_of_jobs = getNumberofJobs(file_name);
  // Parse the input file
  JobQueue *job_arrival_q = readInput(file_name);

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
  simulation->total_jobs = number_of_jobs;
  simulation->getArrivingJobs = getArrivingJobs;
  simulation->scheduler = scheduler;
  simulation->arrivalQueue = job_arrival_q;
  simulation->done = 0;
  simulation->completed = newJobQueue();
  return simulation;
}

JobQueue *getArrivingJobs(void *self, int time)
{
  JobQueue *q = newJobQueue();
  JobQueue *arrivalq = ((Simulation *)self)->arrivalQueue;
  int count = 0;
  while (!isEmpty(arrivalq) && peek(arrivalq)->arrivalTime == time)
  {
    Job *job = dequeue(((Simulation *)self)->arrivalQueue);
    enqueue(q, job);
    count++;
  }
  return q;
}
void logStatistics(Simulation *simulation)
{
  const char *headerline1 = "          | Total Time      | Total time      | Total time      |\n";
  const char *headerline2 = "Job#      | in ready to run | in sleeping on  | in system       |\n";
  const char *headerline3 = "          | state           | I/O state       |                 |\n";
  const char *headerline4 = "==========+=================+=================+=================+\n";
  printf("%s%s%s%s", headerline1, headerline2, headerline3, headerline4);
  while(simulation->n_completed_jobs--)
  {
    Job *completed_job = dequeue(simulation->completed);
    if (!completed_job)
    {
      break;
    }
    char *formatterString = malloc(sizeof(char) * 1000);

    if (!formatterString)
    {
      printf("Malloc Failed");
      exit(1);
    }
    sprintf(formatterString, "%d      | %d               | %d               | %d               |\n",
            completed_job->pid,
            completed_job->timeInReadyToRunState,
            completed_job->timeWaitingForIO,
            completed_job->timeInReadyToRunState + completed_job->timeWaitingForIO + completed_job->timeInRunningState);
    printf("%s", formatterString);
  }
}

int checkIOCompletion(Simulation *simulation, Scheduler *scheduler, int clock)
{

  if (!isEmpty(scheduler->waiting_for_IO_queue))
  {
    // printf("Checking IO completions at time t = %d\n", clock);
    JobQueue *new_queue = newJobQueue();
    while (!isEmpty(scheduler->waiting_for_IO_queue))
    {
      Job *job = dequeue(scheduler->waiting_for_IO_queue);
      int status = IO_complete();
      if (status == 1)
      {
        //  This job is done with IO
        printf("\tJob PID: %d completed IO at time t = %d\n", job->pid, clock);
        job->status = READY;
        scheduler->addJob(scheduler, job);
      }
      else
      {
        // Not done with IO
        printf("\tJob PID: %d not done with IO at time t = %d\n", job->pid, clock);
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

int add_ready_to_run_job(Simulation* s, int clock)
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
      printf("\tAdding Job PID: %d at time t = %d\n", j->pid, clock);

    }
    s->scheduler->addJob(s->scheduler, j);
  }
  return 0;
}