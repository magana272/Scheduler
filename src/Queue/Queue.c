#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Job/Job.h"
#include "../Scheduler/Scheduler.h"

#define GREEN  "\x1B[32m"
#define WHITE  "\x1B[37m"


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

JobQueue *getArrivalQueueFromFile(char *file_name)
{
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
Job* peek(JobQueue* q){
    if(isEmpty(q)){
        return NULL;
    }
    return q->jobs[q->front];
}
void printQueue(JobQueue* q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return;
    }
    printf("%s\n \t\t",GREEN);
    for(int i = 0; i < q->size; i++){
        int index = (q->front + i) % q->capacity;
        Job* job = q->jobs[index];
        if(i < q->size - 1){
            printf("[PID: %d, A: %d, D: %d, L: %d]->", job->pid, job->arrivalTime, job->duration, job->timeRemaining);
        }
        else{
            printf("[PID: %d, A: %d, D: %d, L: %d]\n%s", job->pid, job->arrivalTime, job->duration, job->timeRemaining, WHITE);
        }


    }
}
int isEmpty(JobQueue* q){
    if (q==NULL){
        return 1;
    }
    return q->size == 0;
}
int isFull(JobQueue* q){
    return q->size == q->capacity;
}
int enqueue(JobQueue* q, Job* j){
    if(isFull(q)){
        return -1; // Queue is full
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->jobs[q->rear] = j;
    q->size++;
    return 0; // Success
}
Job* dequeue(JobQueue* q){
    if(q == NULL) {
        return NULL;
    }
    if(isEmpty(q)){
        return NULL; // Queue is empty
    }
    Job* job = q->jobs[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return job;
}
JobQueue* newJobQueue(){
    JobQueue* q = malloc(sizeof(JobQueue));
    if(!q){
        printf("Malloc Failed");
        exit(1);
    }
    q->capacity = 100;
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->jobs = malloc(sizeof(Job*) * q->capacity);
    if(!q->jobs){
        printf("Malloc Failed");
        exit(1);
    }
    return q;
}
JobQueue** newJobQueues(int levels){
    JobQueue** queues = malloc(sizeof(JobQueue*) * MAX_LEVELS);
    if(!queues){
        printf("Malloc Failed");
        exit(1);
    }
    for(int i = 0; i < MAX_LEVELS; i++){
        queues[i] = newJobQueue();
    }
    return queues;
}