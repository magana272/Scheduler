#include <stdio.h>
#include "../src/Utils/Utils.h"
#include "../src/Scheduler/Scheduler.h"

int validate_args_test(int argc, char *argv[])
{

    int result = validate_args(argc, argv);
    if (result == -1)
    {
        printf("Argument validation failed.\n");
    }
    else
    {
        printf("Argument validation succeeded.\n");
    }
    return result;
}
void getFileAndScheduleType_test(char **file_input, ScheduleType *schedule_type, int argc, char *argv[])
{
    int verbose_mode = 0;
    getFileAndScheduleType(file_input, schedule_type, &verbose_mode, argc, argv);
    printf("File Input: %s\n", *file_input);
    printf("Schedule Type: %d\n", *schedule_type);
    printf("Verbose Mode: %d\n", verbose_mode);
}

int main()
{
    char *file_input = "test.txt";
    ScheduleType schedule_type;
    int argc = 2;
    char *argv[] = {"./test", "RR"};

    getFileAndScheduleType_test(&file_input, &schedule_type, argc, argv);

    return 0;
}