#ifndef SIMULATION_LOGGER_H
#define SIMULATION_LOGGER_H
#include "../Simulation/Simulation.h"
void logPreemption(Simulation *simulation, Job *job, Job *current_job, int clock);
void logJobArrival(Simulation *simulation, Job *job, int clock);
void logSimulationDetails(Simulation *simulation, int clock, char *file_input);
void logStatistics(Simulation *simulation);
void logIORequest(Simulation *simulation, Job *job, int clock);
void logJobCompletion(Simulation *simulation, Job *job, int clock);
void logTimeSliceCompletion(Simulation *simulation, Job *job, int clock);
void logBoosting(Simulation *simulation, int clock);

void logTimeDetails(Simulation *simulation, Job *job, int clock);
#endif