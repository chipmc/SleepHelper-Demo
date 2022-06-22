// Particle functions

#ifndef TAKE_MEASUREMENTS_H
#define TAKE_MEASUREMENTS_H


extern char tempString[16];

bool readTempC();           // This is the function that configures the Sleep Helper Function
bool batteryState();        // Data on state of charge and battery status. Returns true if SOC over 60%
bool isItSafeToCharge();    // See if it is safe to charge based on the temperature

#endif