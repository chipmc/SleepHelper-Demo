// Particle functions

#ifndef PARTICLE_FN_H
#define PARTICLE_FN_H

#include "Particle.h"

void particleInitialize();
int setWakeTime(String command); 
int setSleepTime(String command);
int setEnableSleep(String command);

#endif