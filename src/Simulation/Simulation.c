#include "./Simulation.h"
#include "../Queue/Queue.h"
#include "../Scheduler/Scheduler.h"
#include "../Job/Job.h"
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
  simulation->completed = malloc(sizeof(Job) * number_of_jobs);
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
void logStatitics(Simulation simulation)
{
  const char *headerline1 = "          | Total Time      | Total time      | Total time      |\n";
  const char *headerline2 = "Job#      | in ready to run | in sleeping on  | in system       |\n";
  const char *headerline3 = "          | state           | I/O state       |                 |\n";
  const char *headerline4 = "==========+=================+=================+=================+\n";
  printf("%s%s%s%s", headerline1, headerline2, headerline3, headerline4);
  for (int i = 0; i < simulation.n_completed_jobs; i++)
  {
    char *formatterString = malloc(sizeof(char) * 67);
    sprintf(formatterString, "Job%d      | %d               | %d               | %d               |\n",
            simulation.completed[i]->pid,
            simulation.completed[i]->timeInReadyToRunState,
            simulation.completed[i]->timeWaitingForIO,
            simulation.completed[i]->duration);
    printf("%s", formatterString);
  }
}