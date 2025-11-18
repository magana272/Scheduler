
# $$\text{Project 2: Scheduling}$$
This project implements a multi-level feedback scheduling (MLFS) algorithm along with Round Robin (RR) and Preemptive Shortest Job First (SJF) scheduling algorithms.


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

# $\text{Project Structure}$
The project is organized into several directories:

- `src/`: Contains the source code for the simulation, scheduler, job management, and utility functions.
  - `Simulation/`: Manages the overall simulation process.
  - `Scheduler/`: Implements the scheduling algorithms.
  - `Job/`: Defines the job structure and related functions.
  - `Queue/`: Implements the job queue data structure.
  - `IO/`: Handles input/output operations for jobs.
  - `Utils/`: Contains utility functions for argument parsing and validation.
- `test/`: Contains test cases for various components of the project.
- `build/`: Directory where the compiled binaries are stored.
- `testinput/`: Contains input files for testing the scheduling algorithms.

# $\text{Components}$

- **Simulation**: Manages the overall simulation process, including job arrivals and completions.
- **Scheduler**: Implements the scheduling algorithms (MLFS, RR, SJF).
- **Job**: Represents individual jobs with their attributes and states.
- **Utils**: Provides utility functions for argument parsing and validation.

# $\text{Testing}$
To run the tests, use the following command:

```bash
make test
```
This will compile and execute the test cases located in the `test/` directory.

# $$\text{Authors}$$
Please refer to Scheduler.pdf for complete details about the project and its authors.