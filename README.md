
# $$\text{Project 2: CPU Scheduling Simulator}$$
#### $$\text{Manuel Magana, Bhoomika Panday, Tridev Prabhu}$$


This project implements a comprehensive CPU scheduling simulator that supports three different scheduling algorithms:

## **Scheduling Algorithms**

- **`Multi-Level Feedback Scheduling (MLFS)`** - A preemptive algorithm with multiple priority levels
  - **Level 0**: `2 time units` quantum (highest priority)
  - **Level 1**: `10 time units` quantum (medium priority)
  - **Level 2**: `20 time units` quantum (lowest priority)
  - **Boosting**: Jobs promoted to highest priority every `40 time units`

- **`Round Robin (RR)`** - A time-sliced preemptive algorithm
  - **Quantum**: `5 time units`

- **`Shortest Job First (SJF)`** - A preemptive algorithm that prioritizes jobs with shorter remaining time
  - **Strategy**: Preemption based on remaining execution time

The simulator provides detailed statistics and supports verbose mode for debugging and analysis.


# $\text{Running}$
To compile and run the project, use the following commands in your terminal:

```bash
make
./build/scheduler [filepath] [MLFS/RR/SJF] [OPTIONAL: --verbose]
```

There are various test input files available in the ```/testinput``` directory.
For example, to run the MLFS scheduler on the `longinput.txt` file, use the following command:
## Example Output
```shell-session
supercomputer@user:~$make
supercomputer@user:~$./build/scheduler ./testinput/longinput.txt MLFS
==============================================================
#                   Simulation Statistics:                   #
==============================================================
       | Total Time      | Total time      | Total time      |
Job#   | in ready to run | in sleeping on  | in system       |
       | state           | I/O state       |                 |
=======+=================+=================+=================+
40     | 10              | 0               | 10              |
30     | 11              | 5               | 16              |
90     | 9               | 0               | 9               |
60     | 18              | 0               | 18              |
80     | 16              | 0               | 16              |
20     | 32              | 4               | 36              |
50     | 32              | 0               | 32              |
10     | 46              | 3               | 49              |
70     | 30              | 0               | 30              |
==============================================================
Total simulation time     ---------------- 62
Total number of jobs      ---------------- 9
Shortest job completion time ------------- 6
Longest job completion time (CT-AT) ------ 49
Average ready-to-run time ---------------- 22
Average sleeping on I/O   ---------------- 1
Average completion time   ---------------- 21
==============================================================
```

## Example Output with ```-verbose``` flag
```shell-session
make
./build/scheduler ./testinput/SJF.txt SJF --VERBOSE
Verbose mode enabled.



##########################################
# Simulation created successfully...     #
# Scheduling Algorithm: Shortest Job First (SJF) #
# Is Preemptive Scheduler?: No           #
# Total Jobs to complete: 2              #
# Quanta per level:       5 0 0          #
# ./testinput/SJF.txt                    #
##########################################
Running Simulation...
        Job (PID: 10) arrived at time t = 0
        Adding job PID: 10 to SJF heap at time t = 0
        Current SJF Heap:
Time: 0 | Current Job PID: 10 | Current Job Time Remaining: 10 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap:
Time: 1 | Current Job PID: 10 | Current Job Time Remaining: 9 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap:
        Job (PID: 20) arrived at time t = 1
        Adding job PID: 20 to SJF heap at time t = 1
        Current SJF Heap:
        [Preempting] Job PID: 10 with Job PID: 20
Time: 2 | Current Job PID: 20 | Current Job Time Remaining: 4 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap: 10
Time: 3 | Current Job PID: 20 | Current Job Time Remaining: 3 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap: 10
Time: 4 | Current Job PID: 20 | Current Job Time Remaining: 2 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap: 10
        Job PID: 20 made IO request at time t = 4
Time: 4 | Current Job PID: 10 | Current Job Time Remaining: 9 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap:
        Job PID: 20 completed at time t = 4
        [Preempting] Job PID: 10 with Job PID: 20
Time: 5 | Current Job PID: 20 | Current Job Time Remaining: 1 | Current Job Priority: 0 | Completed Jobs: 0
Current SJF Heap: 10
        Job PID: 20 completed at time t = 6
Time: 6 | Current Job PID: 10 | Current Job Time Remaining: 9 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 7 | Current Job PID: 10 | Current Job Time Remaining: 8 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 8 | Current Job PID: 10 | Current Job Time Remaining: 7 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
        Job PID: 10 made IO request at time t = 8
Time: 8
Time: 9
        Job PID: 10 completed at time t = 9
Time: 10 | Current Job PID: 10 | Current Job Time Remaining: 7 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 11 | Current Job PID: 10 | Current Job Time Remaining: 6 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 12 | Current Job PID: 10 | Current Job Time Remaining: 5 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 13 | Current Job PID: 10 | Current Job Time Remaining: 4 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 14 | Current Job PID: 10 | Current Job Time Remaining: 3 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 15 | Current Job PID: 10 | Current Job Time Remaining: 2 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
        Job PID: 10 made IO request at time t = 15
Time: 15
        Job PID: 10 completed at time t = 15
Time: 16 | Current Job PID: 10 | Current Job Time Remaining: 2 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
Time: 17 | Current Job PID: 10 | Current Job Time Remaining: 1 | Current Job Priority: 0 | Completed Jobs: 1
Current SJF Heap:
        Job PID: 10 completed at time t = 18
==============================================================
#                   Simulation Statistics:                   #
==============================================================
       | Total Time      | Total time      | Total time      |
Job#   | in ready to run | in sleeping on  | in system       |
       | state           | I/O state       |                 |
=======+=================+=================+=================+
20     | 0               | 1               | 1               |
10     | 5               | 3               | 8               |
==============================================================
Total simulation time     ---------------- 18
Total number of jobs      ---------------- 2
Shortest job completion time ------------- 5
Longest job completion time (CT-AT) ------ 18
Average ready-to-run time ---------------- 2
Average sleeping on I/O   ---------------- 2
Average completion time   ---------------- 11
==============================================================
```

## Project Architecture

### Directory Structure
```
Project3/
├── src/                     # Source code
│   ├── main.c              # Main simulation loop
│   ├── Simulation/         # Simulation management
│   │   ├── Simulation.c    # Core simulation logic
│   │   ├── Simulation.h    # Simulation interface
│   │   ├── SimulationLogger.c  # Statistics and logging
│   │   └── SimulationLogger.h  # Logger interface
│   ├── Scheduler/          # Scheduling algorithms
│   │   ├── Scheduler.c     # MLFS, RR, and SJF implementations
│   │   └── Scheduler.h     # Scheduler interface
│   ├── Job/                # Job management
│   │   ├── Job.c           # Job creation and lifecycle
│   │   └── Job.h           # Job structure definition
│   ├── Queue/              # Data structures
│   │   ├── Queue.c         # Circular queue implementation
│   │   ├── Queue.h         # Queue interface
│   │   ├── MinHeap.c       # Min-heap for SJF scheduling
│   │   └── MinHeap.h       # Min-heap interface
│   ├── IO/                 # I/O simulation
│   │   ├── IO.c            # Random I/O request generation
│   │   └── IO.h            # I/O interface
│   └── Utils/              # Utilities
│       ├── Utils.c         # Argument parsing and validation
│       └── Utils.h         # Utility functions interface
├── test/                   # Unit tests
├── testinput/             # Sample input files
├── build/                 # Compiled binaries
└── makefile               # Build configuration
```

### Core Components

#### 1. **Simulation Engine** (`src/Simulation/`)
- **Simulation.c**: Manages the overall simulation lifecycle, job arrivals, and queue management
- **SimulationLogger.c**: Provides detailed logging, statistics calculation, and verbose output
- Handles job transitions between different states (READY, RUNNING, WAITING_FOR_IO, COMPLETE)

#### 2. **Scheduling Algorithms** (`src/Scheduler/`)
Three distinct scheduling algorithms implemented with their own strategies:

- **Round Robin (RR)**:
  - Time quantum: 5 time units
  - Non-preemptive except for time slice expiration
  - FIFO queue management

- **Shortest Job First (SJF)**:
  - Preemptive based on remaining execution time
  - Uses min-heap data structure for efficient job selection
  - Prioritizes jobs with shorter remaining time

- **Multi-Level Feedback Scheduling (MLFS)**:
  - Three priority levels (0, 1, 2) with different time quanta
  - Level 0: 2 time units, Level 1: 10 time units, Level 2: 20 time units
  - Jobs demoted to lower priority levels when time slice expires
  - Periodic boosting every 40 time units moves all jobs to highest priority

#### 3. **Job Management** (`src/Job/`)
- Job lifecycle management with states: READY, RUNNING, WAITING_FOR_IO, COMPLETE
- Tracks detailed statistics: arrival time, duration, completion time, wait times
- Memory management with proper cleanup functions

#### 4. **Data Structures** (`src/Queue/`)
- **Queue.c**: Circular queue implementation for job management
- **MinHeap.c**: Priority queue for SJF algorithm based on remaining execution time
- Both structures support dynamic resizing and efficient operations

#### 5. **I/O Simulation** (`src/IO/`)
- Probabilistic I/O request generation during job execution
- Random I/O completion times to simulate realistic system behavior
- Jobs transition to WAITING_FOR_IO state during I/O operations

### Input File Format
Jobs are defined in text files with the format:
```
PID:ARRIVAL_TIME:DURATION:PRIORITY
```

Example:
```
10:0:15:0    # Job PID=10, arrives at time 0, duration 15, priority 0
20:2:8:1     # Job PID=20, arrives at time 2, duration 8, priority 1
```
## Testing & Validation

### Unit Tests
```bash
make test
```

Individual component tests are available in the `test/` directory:
- `IO_test.c` - I/O simulation testing
- `MinHeap_test.c` - Priority queue validation
- `Queue_test.c` - Circular queue operations
- `Scheduler_test.c` - Algorithm correctness
- `Simulation_test.c` - End-to-end simulation tests
- `Utils_test.c` - Utility function validation

### Memory Leak Detection
The project has been tested with Valgrind to ensure zero memory leaks:
```bash
valgrind --tool=memcheck --leak-check=full ./build/scheduler [input] [algorithm]
```

### Performance Metrics
The simulator tracks comprehensive statistics:
- **Turnaround Time**: Time from job arrival to completion
- **Waiting Time**: Time spent in ready queue
- **Response Time**: Time to first CPU allocation
- **I/O Wait Time**: Time spent waiting for I/O completion
- **CPU Utilization**: Percentage of time CPU is busy

## Key Data Structures

### Job Structure
```c
typedef enum {
    READY,
    RUNNING,
    WAITING_FOR_IO,
    COMPLETE
} Status;

typedef struct Job {
    int pid;                    // Process ID
    int arrivalTime;           // Time when job arrives
    int duration;              // Total execution time needed
    int priority;              // Priority level (0=highest)
    int completionTime;        // Time when job completed
    Status status;             // Current job state

    int timeSlice;             // Current time slice for job
    int timeRemaining;         // Time left to complete job
    int timeInReadyToRunState; // Total time in ready queue
    int timeWaitingForIO;      // Total time waiting for I/O
    int timeInRunningState;    // Total time executing
} Job;
```

### Job Queue
```c
typedef struct JobQueue {
    Job **jobs;               // Array of job pointers
    int capacity;             // Maximum queue size
    int front;                // Front index
    int rear;                 // Rear index
    int size;                 // Current number of jobs
} JobQueue;

// Queue operations
JobQueue *newJobQueue();
int enqueue(JobQueue *q, Job *j);
Job *dequeue(JobQueue *q);
Job *peek(JobQueue *q);
int isEmpty(JobQueue *q);
```

### Min-Heap for SJF Scheduling
```c
typedef struct JobMinHeap {
    Job **jobs;               // Array of job pointers
    int capacity;             // Maximum heap size
    int size;                 // Current number of jobs
} JobMinHeap;

// Heap operations - O(log n) complexity
JobMinHeap *newJobMinHeap(int capacity);
int insertJobMinHeap(JobMinHeap *heap, Job *job);
Job *extractMinJobMinHeap(JobMinHeap *heap);
void freeJobMinHeap(JobMinHeap *heap);
```

### Scheduler Structure
```c
typedef struct Scheduler {
    ScheduleType scheduleType;        // RR, SJF, or MLFS
    JobQueue **Levels;                // Multi-level queues (MLFS/RR)
    JobMinHeap *sjf_heap;            // Min-heap for SJF
    JobQueue *waiting_for_IO_queue;   // Jobs waiting for I/O

    int quantum[MAX_LEVELS];          // Time quantum for each level
    int current_quantum;              // Current time quantum
    int boost_interval;               // Boosting frequency (MLFS)

    // Function pointers for algorithm-specific operations
    Job* (*getNextJob)(struct Scheduler* self);
    int (*addJob)(struct Scheduler* self, Job* job);
    int (*preemptive)(struct Scheduler* self, Job* job);
    int (*increment_ready_to_run_state)(struct Scheduler* self);
} Scheduler;
```

### Simulation Control Structure
```c
typedef struct Simulation {
    Scheduler *scheduler;             // The scheduling algorithm
    JobQueue *arrivalQueue;          // Jobs waiting to arrive
    JobQueue *completed;             // Completed jobs

    int total_jobs;                  // Total number of jobs
    int n_completed_jobs;            // Number of completed jobs
    int total_simulation_time;       // Total runtime
    int verbose_mode;                // Debug output flag

    // Function pointer for job arrival handling
    JobQueue* (*getArrivingJobs)(struct Simulation* self, int time);
} Simulation;
```

