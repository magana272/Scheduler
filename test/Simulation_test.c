#include <stdio.h>
#include "../src/Simulation/Simulation.h"
#include "../src/Queue/Queue.h"
#include "../src/Queue/MinHeap.h"
#include "../src/Scheduler/Scheduler.h"
#include "../src/Job/Job.h"

int test_read_input();

int main(){

  Simulation* simulation = newSimulation("testinput/input.txt", RR);

  if(simulation == NULL){
    printf("Test Failed: Simulation creation returned NULL\n");
    return 1;
  }
  test_read_input();
  // printf("Simulation created successfully with total jobs: %d\n", simulation->total_jobs);
  // JobQueue* arrivingJobs = simulation->getArrivingJobs(simulation, 0);
  // Job* job = dequeue(arrivingJobs);
  // printf("First arriving job at time 0: PID=%d, ArrivalTime=%d, Duration=%d\n", job->pid, job->arrivalTime, job->duration);


  return 0;
}

int test_read_input(){
    JobQueue* jobQueue = readInput("testinput/input.txt");
    if(jobQueue == NULL){
        printf("Test Failed: readInput returned NULL\n");
        return 1;
    }
    printf("Jobs read successfully. Total jobs: %d\n", jobQueue->size);
    for(int i = 0; i < jobQueue->size; i++){
        Job* job = jobQueue->jobs[i];
        printf("Job %d: PID=%d, ArrivalTime=%d, Duration=%d\n", i+1, job->pid, job->arrivalTime, job->duration);
    }
    return 0;
}