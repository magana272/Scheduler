#pragma once
#include "../Scheduler/Scheduler.h"
#include "../Job/Job.h"
typedef struct Simulation Simulation;
struct Simulation
{
    Scheduler *scheduler;
    JobQueue *completed;
    JobQueue *arrivalQueue;
    JobQueue *(*getArrivingJobs)(void *self, int time);
    int n_completed_jobs;
    int done;
    int total_jobs;
};

Job *parseJob(char *line);
JobQueue *readInput(char *file_name);
int checkIOCompletion(Simulation *simulation, Scheduler *scheduler, int clock);
Simulation *newSimulation(char *file_name, ScheduleType schedule_type);
JobQueue *getArrivingJobs(void *self, int time);
int getNumberofJob(char *file_name);
void logStatistics(Simulation *simulation);
int isComplete(Simulation *simulation);
int add_ready_to_run_job(Simulation* s, int clock);
