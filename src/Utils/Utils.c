#include "Utils.h"
#include <string.h>
#include <stdio.h>
void getFileAndScheduleType(char **file_input, ScheduleType *schedule_type, int *verbose_mode, int argc, char *argv[])
{

    *file_input = argc > 1 ? argv[1] : "./testinput/input.txt";
    if (argc > 2)
    {
        if (strcmp(argv[2], "RR") == 0)
        {
            *schedule_type = RR;
        }
        else if (strcmp(argv[2], "SJF") == 0)
        {
            *schedule_type = SJF;
        }
        else if (strcmp(argv[2], "MLFS") == 0)
        {
            *schedule_type = MLFS;
        }
        else
        {
            *schedule_type = RR; // Default to RR if unrecognized
        }
    }
    else
    {
        *schedule_type = RR; // Default to RR if not provided
    }
    if (argc == 4){
        if (strcmp(argv[3], "--verbose") == 0){
            *verbose_mode = 1;
        }
    }
}

int validate_args(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <input_file> [SCHEDULER_TYPE]\n", argv[0]);
        printf("SCHEDULER_TYPE can be RR, SJF, or MLFS. Default is RR.\n");
        printf("Example: %s ./testinput/input.txt SJF\n", argv[0]);
        printf("       %s ./testinput/input.txt MLFS\n", argv[0]);
        printf("       %s ./testinput/input.txt RR\n", argv[0]);

        printf("--verbose option can be added as a third argument for detailed logging.\n");
        return -1;
    }

    argv[1] = argv[1]; // Input file
    if (argc > 2)
        {
            if (strcmp(argv[2], "RR") != 0 && strcmp(argv[2], "SJF") != 0 && strcmp(argv[2], "MLFS") != 0)
            {
                printf("Invalid SCHEDULER_TYPE. Must be RR, SJF, or MLFS.\n");
                return -1;
            }
        }

    if (argc == 4){
        if (strcmp(argv[3], "--verbose") == 0){
            printf("Verbose mode enabled.\n");
        }
    }

    return 0;
}
