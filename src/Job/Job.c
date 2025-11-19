#include "Job.h"
#include "../IO/IO.h"
#include <stdio.h>
#include <stdlib.h>

Job *createJob(int pid, int arrivalTime, int duration, Status status)
{
  Job *job = malloc(sizeof(Job));
  job->pid = pid;
  job->arrivalTime = arrivalTime;
  job->duration = duration;
  job->status = READY;
  return job;
}

void destroyJob(Job *job)
{
  if (job != NULL) {
    free(job);
  }
}
int isJobComplete(Job *job)
{

  if (job == NULL)
  {
    return 0;
  }
  else
  {
    return job->timeRemaining <= 1;
  }
}

int checkIfMadeIORequest(Job *job, int clock)
{
  if (job == NULL)
  {
    return 0;
  }
  int status = IO_request();
  if (status == 1)
  {
    job->status = WAITING_FOR_IO;
    return 1;
  }
  return 0;
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
