/**
 * @file particle_fn.h
 * @author Chip McClelland (chip@seeinisghts.com)
 * @brief This file initilizes the Particle functions and variables needed for control from the console / API calls
 * @version 0.1
 * @date 2022-06-30
 * 
 */

// Particle functions
#ifndef PARTICLE_FN_H
#define PARTICLE_FN_H

#include "Particle.h"
#include "storage_objects.h"

// Variables
extern char tempString[16];
extern char currentPointRelease[6];


void particleInitialize();
int setWakeTime(String command); 
int setSleepTime(String command);
int setEnableSleep(String command);

#endif