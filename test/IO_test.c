#include "../src/IO/IO.h"
#include "../src/Job/Job.h"
#include <stdio.h>
#include <stdlib.h>

int IO_REQUEST_test()
{
  Job *job = malloc(sizeof(Job));
  job->pid = 1;
  job->arrivalTime = 0;
  job->duration = 10;
  job->priority = 1;
  job->status = READY;
  job->timeRemaining = job->duration;
  int io_request_count = 0;
  int no_io_request_count = 0;
  int iterations = 10000;
  for (int i = 0; i < iterations; i++) {
    int status = IO_request();
    if (status == 1) {
      io_request_count++;
    } else {
      no_io_request_count++;
      job->timeRemaining--;
      if (job->timeRemaining == 0) {
        job->status = COMPLETE;
        job->timeRemaining = job->duration;
      }
      job->timeRemaining--;

    }
  }
  printf("Out of %d iterations, IO requests: %d, No IO requests: %d\n", iterations, io_request_count, no_io_request_count);
}

int main(){
  srand(0);
  IO_REQUEST_test();
  return 0;
}