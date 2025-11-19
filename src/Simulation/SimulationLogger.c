#include "SimulationLogger.h"
#include "../Simulation/Simulation.h"
#include "../Job/Job.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define RED "\x1B[31m"
#define WHITE "\x1B[37m"
#define BLUE "\x1B[34m"
#define GREEN "\x1B[32m"

void logRRDetails(Scheduler *scheduler)
{
    printf("\tCurrent RR Queue: ");
    JobQueue *rr_queue = scheduler->Levels[0];
    for (int i = 0; i < rr_queue->size; i++)
    {
        printf("%d ", rr_queue->jobs[i]->pid);
    }
    printf("\n");
}
void logTimeDetails(Simulation *simulation, Job *job, int clock)
{
    if (simulation->verbose_mode)
    {
        if(simulation->scheduler->scheduleType == SJF) {
            printf("%sTime: %d | Current Job PID: %d | Current Job Time Remaining: %d | Current Job Priority: %d | Completed Jobs: %d%s\n",
               BLUE, clock,
               job->pid,
               job->timeRemaining,
               job->priority,
               simulation->n_completed_jobs,
               WHITE);
            return;
        }
        printf("%sTime: %d | Current Job PID: %d | Current Job Time Remaining: %d | Current Job Priority: %d\nCompleted Jobs: %d| Curr Time Slice: %d%s\n",
               BLUE, clock,
               job->pid,
               job->timeRemaining,
               job->priority,
               simulation->n_completed_jobs,
               job->timeSlice,
               WHITE);
    }
}
void logSJFDetails(Scheduler *scheduler)
{
    printf("\tCurrent SJF Heap: ");
    JobMinHeap *sjf_heap = scheduler->sjf_heap;
    for (int i = 0; i < sjf_heap->size; i++)
    {
        printf("%d ", sjf_heap->jobs[i]->pid);
    }
    printf("\n");
}
void logMLFSDetails(Scheduler *scheduler)
{
    printf("\tCurrent MLFS Queues:\n");
    for (int level = 0; level < MAX_LEVELS; level++)
    {
        printf("\t Level %d Queue: ", level);
        JobQueue *level_queue = scheduler->Levels[level];
        for (int i = 0; i < level_queue->size; i++)
        {
            printf("%d ", level_queue->jobs[i]->pid);
        }
        printf("\n");
    }
}
void logJobArrival(Simulation *simulation, Job *job, int clock)
{
    if (simulation->verbose_mode)
    {
        printf("\tJob (PID: %d) arrived at time t = %d\n", job->pid, clock);
        if (simulation->scheduler->scheduleType == RR)
        {
            printf("\t%sAdding job PID: %d to RR queue at time t = %d\n%s", BLUE, job->pid, clock, WHITE);
            logRRDetails(simulation->scheduler);
        }
        else if (simulation->scheduler->scheduleType == SJF)
        {
            printf("\t%sAdding job PID: %d to SJF heap at time t = %d\n%s", BLUE, job->pid, clock, WHITE);
            logSJFDetails(simulation->scheduler);
        }
        else if (simulation->scheduler->scheduleType == MLFS)
        {
            printf("\t%sAdding job PID: %d to MLFS Level %d queue at time t = %d\n%s", BLUE, job->pid, job->priority, clock, WHITE);
            logMLFSDetails(simulation->scheduler);
        }
    }
}
void logPreemption(Simulation *simulation, Job *job, Job *current_job, int clock)
{
    if (simulation->verbose_mode)
    {
        printf("\t%s[Preempting] Job PID: %d with Job PID: %d\n%s", RED, job->pid, current_job->pid, WHITE);
    }
}
void logSimulationDetails(Simulation *simulation, int clock, char *file_input)
{
    if (simulation->verbose_mode)
    {
        char *schedule_type_str;
        Scheduler *scheduler = simulation->scheduler;

        int preemptive_scheduler =
            (scheduler->scheduleType == SJF) ||
            (scheduler->scheduleType == MLFS);

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

        const int INNER = 38;

        printf("\n\n\n");
        printf("##########################################\n");

        printf("# %-38s #\n", "Simulation created successfully...");
        if (strcmp(schedule_type_str, "SJF") == 0)
        {
            printf("# %-38s #\n", "Scheduling Algorithm: Shortest Job First (SJF)");
        }
        else if (strcmp(schedule_type_str, "RR") == 0)
        {
            printf("# %-38s #\n", "Scheduling Algorithm: Round Robin (RR)");
        }
        else if (strcmp(schedule_type_str, "MLFS") == 0)
        {
            printf("# %-38s #\n", "Scheduling Algorithm: Multi-Level Feedback Queue (MLFS)");
        }
        else
        {
            printf("# %-38s #\n", "Scheduling Algorithm: Unknown");
        }
        if (scheduler->scheduleType == MLFS)
        {
            printf("# %-38s #\n", "Is Preemptive Scheduler?: Yes");
        }
        else
        {
            printf("# %-38s #\n", "Is Preemptive Scheduler?: No");
        }
        printf("# %-22s %-14d #\n", "Total Jobs to complete:", simulation->total_jobs);
        printf("# %-23s ", "Quanta per level:");
        int used = 0;
        for (int i = 0; i < MAX_LEVELS; i++)
            used += printf("%d ", scheduler->quantum[i]);

        int pad = INNER - 22 - 1 - used;
        if (pad < 0)
            pad = 0;
        printf("%*s#\n", pad, "");
        printf("# %-38s #\n", file_input);

        printf("##########################################\n");
    }
}
void logBoosting(Simulation *simulation, int clock)
{
    if (simulation->verbose_mode)
    {
        printf("🚀 Boosting at time t = %d\n", clock);
    }
}
void logTimeSliceCompletion(Simulation *simulation, Job *job, int clock)
{
    if (simulation->verbose_mode)
    {
        printf("\tTime slice completed for job PID: %d at time t = %d\n", job->pid, clock);
    }
}
void logJobCompletion(Simulation *simulation, Job *job, int clock)
{
    if (simulation->verbose_mode)
    {
        printf("\t%sJob PID: %d completed at time t = %d\n%s", GREEN, job->pid, clock, WHITE);
    }
}
void logIORequest(Simulation *simulation, Job *job, int clock)
{
    if (simulation->verbose_mode)
    {
        printf("\t%sJob PID: %d made IO request at time t = %d%s\n", RED, job->pid, clock, WHITE);
    }
}
void logStatistics(Simulation *simulation)
{
    char *formatterString = malloc(sizeof(char) * 1000);
    sprintf(formatterString, "==============================================================\n");
    printf("%s", formatterString);
    sprintf(formatterString, "#%18s Simulation Statistics:%20s\n==============================================================\n", "", "#");
    printf("%s", formatterString);

    const char *headerline1 = "       | Total Time      | Total time      | Total time      |\n";
    const char *headerline2 = "Job#   | in ready to run | in sleeping on  | in system       |\n";
    const char *headerline3 = "       | state           | I/O state       |                 |\n";
    const char *headerline4 = "=======+=================+=================+=================+\n";
    printf("%s%s%s%s", headerline1, headerline2, headerline3, headerline4);

    int total_simulation_time = 0;
    int total_number_of_jobs = simulation->n_completed_jobs;
    int shortest_job_completion_time = __INT_MAX__;
    int longest_job_completion_time = 0;
    int accumulated_ready_time = 0;
    int accumulated_sleeping_time = 0;
    int accumulated_completion_time = 0;
    while (simulation->n_completed_jobs--)
    {
        Job *completed_job = dequeue(simulation->completed);
        if (!completed_job)
        {
            break;
        }
        char *formatterString = malloc(sizeof(char) * 1000);

        if (!formatterString)
        {
            printf("Malloc Failed");
            exit(1);
        }
        sprintf(formatterString, "%s%-6d%s | %s%-15d%s | %s%-15d%s | %s%-15d%s |\n",
                RED, completed_job->pid, WHITE,
                RED, completed_job->timeInReadyToRunState, WHITE,
                RED, completed_job->timeWaitingForIO, WHITE,
                RED, completed_job->completionTime - completed_job->arrivalTime, WHITE);
        printf("%s", formatterString);
        if (completed_job->completionTime - completed_job->arrivalTime < shortest_job_completion_time)
        {
            shortest_job_completion_time = completed_job->completionTime - completed_job->arrivalTime;
        }
        if (completed_job->completionTime - completed_job->arrivalTime > longest_job_completion_time)
        {
            longest_job_completion_time = completed_job->completionTime - completed_job->arrivalTime;
        }
        accumulated_ready_time += completed_job->timeInReadyToRunState;
        accumulated_sleeping_time += completed_job->timeWaitingForIO;
        accumulated_completion_time += completed_job->completionTime - completed_job->arrivalTime;
    }
    printf("==============================================================\n");
    printf("%s%-25s ---------------- %s%d%s\n", BLUE, "Total simulation time", RED, simulation->total_simulation_time, WHITE);
    printf("%s%-25s ---------------- %s%d%s\n", BLUE, "Total number of jobs", RED, total_number_of_jobs, WHITE);
    printf("%s%-22s ------------- %s%d%s\n", BLUE, "Shortest job completion time", RED, shortest_job_completion_time, WHITE);
    printf("%s%-35s ------ %s%d%s\n", BLUE, "Longest job completion time (CT-AT)", RED, longest_job_completion_time, WHITE);
    printf("%s%-25s ---------------- %s%d%s\n", BLUE, "Average ready-to-run time", RED, accumulated_ready_time / total_number_of_jobs, WHITE);
    printf("%s%-25s ---------------- %s%d%s\n", BLUE, "Average sleeping on I/O", RED, accumulated_sleeping_time / total_number_of_jobs, WHITE);
    printf("%s%-25s ---------------- %s%d%s\n", BLUE, "Average completion time", RED, accumulated_completion_time / total_number_of_jobs, WHITE);
    printf("==============================================================\n%s", WHITE);
}
