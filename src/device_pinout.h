
/**
 * @file   device_pinout.h
 * @author Chip McClelland
 * @date   6-16-2022
 * @brief  File containing the pinout documentation for 3rd Generation Particle Devices.
 * */

#ifndef DEVICE_PINOUT_H
#define DEVICE_PINOUT_H

#include "Particle.h"

// Pin definitions (changed from example code)
extern const pin_t TMP36_SENSE_PIN;
extern const pin_t TMP36_POWER_PIN;
extern const pin_t BUTTON_PIN;
extern const pin_t BLUE_LED;
extern const pin_t WAKEUP_PIN;   

bool initializePinModes();

#endif