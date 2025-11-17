#include <stdio.h>
#include <stdlib.h>
#include "../src/Simulation/Simulation.h"
#include "../src/Queue/Queue.h"
#include "../src/Queue/MinHeap.h"
#include "../src/Scheduler/Scheduler.h"
#include "../src/Job/Job.h"
#include <assert.h>

int test_read_input();
int test_add_ready_to_run_job();


int main(){

  Simulation* simulation = newSimulation("testinput/input.txt", RR);

  if(simulation == NULL){
    printf("Test Failed: Simulation creation returned NULL\n");
    return 1;
  }
  // Scheduler* sch = simulation->scheduler;
  // Job* j = malloc(sizeof(Job));
  // j->pid = 1;
  // j->duration = 5;
  // sch->addJob(sch, j);

  test_read_input();
  printf("Testing| Ready to run\n");
  test_add_ready_to_run_job();
  return 0;
}


int test_add_ready_to_run_job(){
  JobQueue* arrivalQ = newJobQueue();
  Job* j1 = createJob(1,0, 2, READY);
  Job* j2 = createJob(2,0, 2, READY);
  enqueue(arrivalQ, j1);
  enqueue(arrivalQ, j2);
  Simulation* simulation = newSimulation("testinput/input.txt", RR);
  Scheduler* sch = simulation->scheduler;
  simulation->arrivalQueue = arrivalQ;
  add_ready_to_run_job(simulation,0);
  Job* j =  sch->getNextJob(sch);
  assert(j->pid == 1);
  j =  sch->getNextJob(sch);
  assert(j->pid == 2);
  return 0;
}
int test_read_input(){
    JobQueue* arrivalQ = readInput("testinput/input.txt");
    if(arrivalQ == NULL){
        printf("Test Failed: readInput returned NULL\n");
        return 1;
    }
    printf("Jobs read successfully. Total jobs: %d\n", arrivalQ->size);
    for(int i = 0; i < arrivalQ->size; i++){
        Job* job = arrivalQ->jobs[i];
        printf("Job %d: PID=%d, ArrivalTime=%d, Duration=%d\n", i+1, job->pid, job->arrivalTime, job->duration);
    }
    return 0;
}