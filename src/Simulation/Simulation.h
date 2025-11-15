#pragma once
#include "../Scheduler/Scheduler.h"
#include "../Job/Job.h"
typedef struct Simulation Simulation;
struct Simulation
{
    Scheduler *scheduler;
    Job **completed;
    JobQueue *arrivalQueue;
    JobQueue *(*getArrivingJobs)(void *self, int time);
    int n_completed_jobs;
    int done;
    int total_jobs;
};

Job *parseJob(char *line);
JobQueue *readInput(char *file_name);
Simulation *newSimulation(char *file_name, ScheduleType schedule_type);
JobQueue *getArrivingJobs(void *self, int time);
int getNumberofJob(char *file_name);
void *logStatistics(Simulation *simulation);
