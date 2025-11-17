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
    exit(1);
  }
  char *file_input;
  ScheduleType schedule_type;
  getFileAndScheduleType(&file_input, &schedule_type, argc, argv);
  Simulation *simulation = newSimulation(file_input, schedule_type);
  Scheduler *scheduler = simulation->scheduler;
  int preemptive_scheduler = (scheduler->scheduleType == SJF) || (scheduler->scheduleType == MLFS);
  char *schedule_type_str;
  switch (scheduler->scheduleType)
  {
  case RR:
    schedule_type_str = "RR";
    break;
  case SJF:
    schedule_type_str = "SJF";
    break;
  case MLFS:
    schedule_type_str = "MLFS";
    break;
  default:
    schedule_type_str = "Unknown";
  }
  printf("\n\n\n");
  printf("########################################\n");
  printf("# Simulation created successfully...  \n");
  printf("# scheduleType:           %s           \n", schedule_type_str);
  printf("# preemptive_scheduler: %s             \n", preemptive_scheduler ? "T" : "F");
  printf("# Total Jobs to complete: %d           \n", simulation->total_jobs);
  printf("# File Input: %s           \n", file_input);
  printf("########################################\n");
  printf("Running Simulation...\n");
  int current_job_time_slice_completed = 0;
  int current_job_preempted = 0;
  int current_job_completed = 0;
  int current_job_made_io_request = 0;
  add_ready_to_run_job(simulation, clock);
  srand(1);
  while (!isComplete(simulation))
  {

    add_ready_to_run_job(simulation, clock);
    Job *current_job = scheduler->getNextJob(scheduler);
    if (current_job == NULL)
    {
      printf("Time: %d\n", clock);
      checkIOCompletion(simulation, scheduler, clock);
      clock++;
    }
    else
    {
      while (1)
      {
        printf("Time: %d\n", clock);
        current_job_time_slice_completed = 0;
        current_job_preempted = 0;
        current_job_completed = 0;
        current_job_made_io_request = 0;
        current_job->status = RUNNING;
        add_ready_to_run_job(simulation, clock);
        checkIOCompletion(simulation, scheduler, clock);
        current_job_completed = isJobComplete(current_job);
        current_job_made_io_request = checkIfMadeIORequest(current_job, clock);
        current_job_preempted = isJobPreempted(scheduler, current_job);
        current_job_time_slice_completed = (current_job->timeSlice == 1) && (scheduler->scheduleType != SJF);

        if (current_job_completed)
        {
          clock++;
          printf("Time: %d\n", clock);
          printf("\t%sJob PID: %d completed at time t = %d\n%s", KGRN, current_job->pid, clock, KWHT);
          current_job->status = COMPLETE;
          simulation->n_completed_jobs += 1;
          current_job_completed = 1;
          current_job->completionTime = clock;
          current_job->timeInRunningState++;
          enqueue(simulation->completed, current_job);
          scheduler->increment_ready_to_run_state(scheduler);
          break;
        }
        if (current_job_preempted)
        {
          printf("\tSwapping out current job PID: %d at time t = %d\n", current_job->pid, clock);
          Job *j = current_job;
          printf("\tGetting next job after preemption at time t = %d\n", clock);
          current_job = scheduler->getNextJob(scheduler);
          scheduler->addJob(scheduler, j);

        }
        if (current_job_made_io_request)
        {
          printf("\t%sJob PID: %d made IO request at time t = %d%s\n", KGRN, current_job->pid, clock, KWHT);
          current_job->status = WAITING_FOR_IO;
          current_job->timeWaitingForIO += 1;
          enqueue(scheduler->waiting_for_IO_queue, current_job);
          scheduler->increment_ready_to_run_state(scheduler);
          break;
        }
        if (current_job_time_slice_completed)
        {
          printf("\tTime slice completed for job PID: %d at time t = %d\n", current_job->pid, clock);
          current_job->status = RUNNING;
          scheduler->addJob(scheduler, current_job);
          scheduler->increment_ready_to_run_state(scheduler);
          break;
        }
        clock++;
        current_job->timeSlice--;
        current_job->timeRemaining--;
        scheduler->increment_ready_to_run_state(scheduler);
        current_job->timeInRunningState++;
      }
    }
  }
  logStatistics(simulation);
}
