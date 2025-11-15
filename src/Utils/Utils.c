#include "Utils.h"
#include <string.h>
void getFileAndScheduleType(char **file_input, ScheduleType *schedule_type, int argc, char *argv[])
{

    *file_input = argc > 1 ? argv[1] : "input.txt";
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
}

int validate_args(int argc, char *argv[])
{
    return 0;
}
