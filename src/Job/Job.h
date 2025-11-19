#pragma once

typedef enum
{
  READY,
  RUNNING,
  WAITING_FOR_IO,
  COMPLETE
} Status;
typedef struct Job
{
  int pid;            // i.e 1
  int arrivalTime;    // i.e  0
  int duration;       // i.e  10
  int priority;       // i.e  1 : equal to level
  int completionTime; // time when job completed
  Status status;      // READY, RUNNING, WAITING_FOR_IO, COMPLETE

  int timeSlice; // current time slice for the job
  // Bookkeeping fields
  int timeRemaining;         // time left to complete job
  int timeInReadyToRunState; // total time in ready to run state
  int timeWaitingForIO;      // total time waiting for I/O
  int timeInRunningState;    // total time in running state

} Job;

Job *createJob(int pid, int arrivalTime, int duration, Status status);
void destroyJob(Job *job);
int checkIfMadeIORequest(Job *job, int clock);
int isJobComplete(Job *job);
Job *parseJob(char *line);