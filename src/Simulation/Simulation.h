#pragma once
#include "../Scheduler/Scheduler.h"
#include "../Job/Job.h"
typedef struct Simulation Simulation;
struct Simulation
{
    Scheduler *scheduler;
    JobQueue *completed;
    JobQueue *arrivalQueue;
    JobQueue *(*getArrivingJobs)(Simulation *self, int time);
    int verbose_mode;
    int n_completed_jobs;
    int done;
    int total_jobs;
    int total_simulation_time;
};

Job *parseJob(char *line);
JobQueue *readInput(char *file_name);
int checkIOCompletion(Simulation *simulation, Scheduler *scheduler, int clock);
Simulation *newSimulation(char *file_name, ScheduleType schedule_type);
JobQueue *getArrivingJobs(Simulation *self, int time);
int getNumberofJob(char *file_name);
int isComplete(Simulation *simulation);
int add_ready_to_run_job(Simulation* s, int clock);
