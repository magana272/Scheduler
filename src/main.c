#include <stdlib.h>
#include <stdio.h>
#include "./Simulation/Simulation.h"
#include "./Simulation/SimulationLogger.h"
#include "./Job/Job.h"
#include "./Utils/Utils.h"
#include "./Scheduler/Scheduler.h"
#include "./Queue/Queue.h"
#include "./IO/IO.h"
int clock = 0;

int main(int argc, char *argv[])
{
  int verbose_mode = 0;
  int err = validate_args(argc, argv);
  if (err)
  {
    exit(1);
  }
  char *file_input;
  ScheduleType schedule_type;
  getFileAndScheduleType(&file_input, &schedule_type, &verbose_mode, argc, argv);
  Simulation *simulation = newSimulation(file_input, schedule_type);
  simulation->verbose_mode = verbose_mode;

  Scheduler *scheduler = simulation->scheduler;
  int preemptive_scheduler = (scheduler->scheduleType == SJF) || (scheduler->scheduleType == MLFS);
  logSimulationDetails(simulation, clock, file_input);
  printf("Running Simulation...\n");
  int current_job_time_slice_completed = 0;
  int current_job_preempted = 0;
  int current_job_completed = 0;
  int current_job_made_io_request = 0;
  add_ready_to_run_job(simulation, clock);
  os_srand(1);
  while (!isComplete(simulation))
  {

    add_ready_to_run_job(simulation, clock);
    Job *current_job = scheduler->getNextJob(scheduler);
    if (current_job == NULL)
    {
      if (simulation->verbose_mode)
      {
        printf("Time: %d\n", clock);
      }
      checkIOCompletion(simulation, scheduler, clock);
      clock++;
    }
    else
    {
      while (1)
      {
        if (simulation->verbose_mode)
        {
          logTimeDetails(simulation, current_job, clock);
          printQueues(scheduler);
        }
        current_job_time_slice_completed = 0;
        current_job_preempted = 0;
        current_job_completed = 0;
        current_job_made_io_request = 0;
        current_job->status = RUNNING;
        add_ready_to_run_job(simulation, clock);
        checkIOCompletion(simulation, scheduler, clock);
        current_job_completed = isJobComplete(current_job);
        if (scheduler->scheduleType == MLFS)
        {
          if (clock % scheduler->boost_interval == 0 && clock != 0)
          {
            if (simulation->verbose_mode)
            {
              logBoosting(simulation, clock);
            }
            boost(scheduler);
          }
        }
        current_job_made_io_request = checkIfMadeIORequest(current_job, clock);
        current_job_preempted = isJobPreempted(scheduler, current_job);
        current_job_time_slice_completed = (current_job->timeSlice == 1) && (scheduler->scheduleType != SJF);

        if (current_job_completed)
        {
          clock++;
          logJobCompletion(simulation, current_job, clock);
          current_job->status = COMPLETE;
          simulation->n_completed_jobs += 1;
          current_job_completed = 1;
          current_job->completionTime = clock;
          current_job->timeInRunningState++;
          enqueue(simulation->completed, current_job);
          break;
        }
        if (current_job_preempted)
        {
          Job *j = current_job;
          current_job = scheduler->getNextJob(scheduler);
          scheduler->addJob(scheduler, j);
          current_job->timeInRunningState++;
          logPreemption(simulation, j, current_job, clock);
        }
        if (current_job_made_io_request)
        {
          logIORequest(simulation, current_job, clock);
          current_job->status = WAITING_FOR_IO;
          current_job->timeWaitingForIO += 1;
          enqueue(scheduler->waiting_for_IO_queue, current_job);
          scheduler->increment_ready_to_run_state(scheduler);
          break;
        }
        if (current_job_time_slice_completed)
        {
          logTimeSliceCompletion(simulation, current_job, clock);
          current_job->status = RUNNING;
          current_job->timeInRunningState++;
          current_job->timeRemaining--;
          // clock++;
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
  simulation->total_simulation_time = clock;
  logStatistics(simulation);
}
