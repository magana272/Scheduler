#include <stdlib.h>
#include <stdio.h>
#include "./Simulation/Simulation.h"
#include "./Job/Job.h"
#include "./Utils/Utils.h"
#include "./Scheduler/Scheduler.h"
#include "./Queue/Queue.h"
#include "./IO/IO.h"

int clock = 0;
int main(int argc, char *argv[])
{
  int err = validate_args(argc, argv);
  if (err)
  {
    // exit nicely
    exit(1);
  }
  char *file_input;
  ScheduleType schedule_type;

  getFileAndScheduleType(&file_input, &schedule_type, argc, argv);
  printf("File Input: %s\n", file_input);
  printf("Schedule Type: %d\n", schedule_type);
  printf("Starting Simulation...\n");
  Simulation *simulation = newSimulation(file_input, schedule_type);
  printf("Simulation created successfully...\n");
  Scheduler *scheduler = simulation->scheduler;
  int preemptive_scheduler = (scheduler->scheduleType == SJF) || (scheduler->scheduleType == MLFS);
  JobQueue *jobs_at_timeT = simulation->getArrivingJobs(simulation, clock);
  if (jobs_at_timeT == NULL)
  {
    printf("No jobs at time t = %d\n", clock);
  }
  while (!isEmpty(jobs_at_timeT))
  {
    printf("I'm here??\n");
    Job *j = dequeue(jobs_at_timeT);
    if(j != NULL){
      printf("Adding Job PID: %d at time t = %d\n", j->pid, clock);
    }
    scheduler->addJob(scheduler, j);
  }
  srand(1);
  clock++;
  printf("Clock: %d\n", clock);
  printf("scheduler->scheduleType: %d\n", scheduler->scheduleType);
  printf("preemptive_scheduler: %d\n", preemptive_scheduler);
  while (!simulation->done)
  {
    printf("Getting next job at time t = %d\n", clock);
    Job *current_job = scheduler->getNextJob(scheduler);
    if (current_job != NULL)
    {
      printf("Current Job PID: %d\n", current_job->pid);
      current_job->timeSlice = scheduler->current_quantum;
    }
    int current_job_was_swapped_out = 0;
    int status;
    while (1)
    {
      /*******************************************************************************
       *
       *     Jobs at time t are added to scheduler
       *
       *
       ******************************************************************************/
      JobQueue *jobs_at_timeT = simulation->getArrivingJobs(simulation, clock);
      // If there are no jobs
      if (jobs_at_timeT == NULL)
      {
        printf("No jobs at time t = %d\n", clock);
      }
      else
      {
        // Add the jobs to the ready to run state
        for (int i = 0; i < jobs_at_timeT->size; i++)
        {
          scheduler->addJob(scheduler, jobs_at_timeT->jobs[i]);
        }
      }

      /************************************************
       *
       * For the Jobs waiting for IO
       * Check if there are done
       *
       *************************************************/
      //      JobQueue *new_queue = NULL; // or list
      if (!isEmpty(scheduler->waiting_for_IO_queue))
      {
        JobQueue *new_queue = newJobQueue();
        while (!isEmpty(scheduler->waiting_for_IO_queue))
        {
          Job *job = dequeue(scheduler->waiting_for_IO_queue);
          int status = IO_complete();
          if (status == 1)
          {
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
      }

      /************************************************
       *   Check to see if the job is complete
       *   current_job->timeRemaining == 1
       *
       *
       *************************************************/
      if (current_job->timeRemaining == 1)
      {
        current_job->status = COMPLETE;
        scheduler->n_completed_jobs += 1;
        simulation->completed[scheduler->n_completed_jobs] = current_job;
        break;
      }
      /************************************************
       *  Check if there are higher priority jobs
       *  (if we are using SJF or MLFS)
       *
       *************************************************/
      if (preemptive_scheduler)
      {
        int higher_priority_jobs_on_the__ready_to_run_state = scheduler->preemptive(scheduler, current_job);
        if (higher_priority_jobs_on_the__ready_to_run_state)
        {
          current_job_was_swapped_out = 1;
        }
      }
      if (current_job->status != COMPLETE) // ?? Check to see if current job needs to be IO bound ??
      {
        status = IO_request();
        if (status == 1)
        {
          current_job->status = WAITING_FOR_IO;
          enqueue(scheduler->waiting_for_IO_queue, current_job); /// Could more later ?
        }
        if (current_job->timeSlice == 1)
        {
          current_job_was_swapped_out = 1;
        }
      }

      if (current_job_was_swapped_out)
      {
        if (current_job->status != COMPLETE && current_job->status != WAITING_FOR_IO)
        {
          current_job->status = READY;
          scheduler->addJob(scheduler, current_job);
          break;
        }
        else
        {
          scheduler->addJob(scheduler, current_job);
        }
      }
      // do bookkeeping and statistics
      clock++;
      current_job->timeSlice--;
      current_job->timeRemaining--;
    }
  }
}
