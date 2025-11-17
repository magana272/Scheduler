#include "Job.h"
#include "../IO/IO.h"
#include <stdio.h>
#include <stdlib.h>


Job* createJob(int pid, int arrivalTime, int duration, Status status){
  Job* job  = malloc(sizeof(Job));
  job->pid = pid;
  job->arrivalTime = arrivalTime;
  job->duration = duration;
  job->status = READY;
  return job;
}
int isJobComplete(Job* job){

  if (job == NULL){
    return 0;
  }
  else{
    return job->timeRemaining == 1;
  }
}

int checkIfMadeIORequest(Job *job, int clock)
{
  if (job== NULL){
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