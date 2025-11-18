#include <stdio.h>
#include "../src/Utils/Utils.h"
#include "../src/Scheduler/Scheduler.h"

int validate_args_test(int argc, char *argv[])
{

    return 0;
}
void getFileAndScheduleType_test(char **file_input, ScheduleType *schedule_type, int argc, char *argv[])
{
}

int main()
{
    char *file_input = "test.txt";
    ScheduleType schedule_type;
    int argc = 2;
    char *argv[] = {"./test", "arg1"};

    getFileAndScheduleType_test(&file_input, &schedule_type, argc, argv);

    return 0;
}