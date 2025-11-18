
#include <stdio.h>
#include "../src/Queue/Queue.h"
#include "../src/Job/Job.h"
#include "../src/Scheduler/Scheduler.h"
#include <assert.h>


void test_newJobQueues() {
    JobQueue** queues = newJobQueues(3);
    for (int i = 0; i < 3; i++) {
        if (queues[i] == NULL) {
            printf("test_newJobQueues FAILED: Queue %d is NULL\n", i);
            return;
        }
        if (queues[i]->size != 0) {
            printf("test_newJobQueues FAILED: Queue %d size is not 0\n", i);
            return;
        }
    }
    printf("test_newJobQueues PASSED\n");

    Scheduler* s  = newScheduler(MLFS);

    s->addJob(s, createJob(1,0,10,READY));
    s->addJob(s, createJob(2,0,15,READY));
    s->addJob(s, createJob(3,0,20,READY));
    // printQueues(s);

    for (int i = 0; i < MAX_LEVELS; i++) {
        // All jobs should be in level 0
        if (s->Levels[i]->size != (i == 0 ? 3 : 0)) {
            printf("test_newJobQueues FAILED: Level %d size is incorrect\n", i);
            return;
        }
    }
    Job* j = s->getNextJob(s);
    if (j->pid != 1) {
        printf("test_newJobQueues FAILED: Expected PID 1, got %d\n", j->pid);
        return;
    }

    for (int i = 0; i < MAX_LEVELS; i++) {
        // Job 1 should be removed from level 0
        if (s->Levels[i]->size != (i == 0 ? 2 : 0)) {
            printf("test_newJobQueues FAILED: Level %d size is incorrect after dequeue\n", i);
            return;
        }
        // Job 1 should be removed from level 0
        while (s->Levels[i]->size > 0) {

            j = dequeue(s->Levels[i]);
            if (j->pid == 1) {
                printf("test_newJobQueues FAILED: Job PID 1 still in Level %d after dequeue\n", i);
                return;
            }
            printf("Dequeued Job PID %d from Level %d\n", j->pid, i);
            printf("\tCurrent MLFS Queues:\n");
            printf("should be empty now\n");
            printQueues(s);
        }
    }


    printf("test_newJobQueues PASSED\n");


}
void test_removeJobAndAddBack(){
  JobQueue** queues = newJobQueues(3);
    for (int i = 0; i < 3; i++) {
        if (queues[i] == NULL) {
            printf("test_newJobQueues FAILED: Queue %d is NULL\n", i);
            return;
        }
        if (queues[i]->size != 0) {
            printf("test_newJobQueues FAILED: Queue %d size is not 0\n", i);
            return;
        }
    }
    printf("test_newJobQueues PASSED\n");

    Scheduler* s  = newScheduler(MLFS);

    s->addJob(s, createJob(5,0,10,READY));
    s->addJob(s, createJob(666,0,15,READY));
    s->addJob(s, createJob(999,0,20,READY));
    Job* j = s->getNextJob(s);
    j->timeRemaining = 5; // Simulate some execution
    s->addJob(s, j);

    if (s->Levels[1]->size != 1) {
        printf("test_removeJobAndAddBack FAILED: Job not added to correct level\n");
        return;
    }
    j = dequeue(s->Levels[1]);
    if (j->pid != 5) {
        printf("test_removeJobAndAddBack FAILED: Expected PID 5, got %d\n", j->pid);
        return;
    }
    printf(s->Levels[0]->size ==2 ? "test_removeJobAndAddBack PASSED\n" : "test_removeJobAndAddBack FAILED: Level 0 size incorrect\n");
    while(s->Levels[0]->size >0){
        j = dequeue(s->Levels[0]);
        printf("Dequeued Job PID %d from Level 0\n", j->pid);
        assert(j->pid == 666 || j->pid == 999);

    }
  }
    int main() {
        // test_newJobQueues();
    test_removeJobAndAddBack();
    return 0;
}