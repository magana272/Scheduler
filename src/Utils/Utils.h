#pragma once
#include "../Scheduler/Scheduler.h"
void getFileAndScheduleType(char **file_input, ScheduleType *schedule_type, int *verbose_mode, int argc, char *argv[]);
int validate_args(int argc, char *argv[]);
