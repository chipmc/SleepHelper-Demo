/**
 * @file sleep_helper_config.h
 * @author Chip McClelland (chip@seeinsights.com)
 * @brief In the sleep_helper_config.cpp file, you can set the parameters that will configure the Sleep Helper function
 * @link https://rickkas7.github.io/SleepHelper/index.html
 * @version 0.1
 * @date 2022-06-17
 * 
 */

#ifndef SLEEP_HELPER_CONFIG_H
#define SLEEP_HELPER_CONFIG_H

#include "Particle.h"
#include "AB1805_RK.h"
#include "PublishQueuePosixRK.h"
#include "SleepHelper.h"
#include "take_measurements.h"
#include "storage_objects.h"
#include "device_pinout.h"

extern AB1805 ab1805;                               // This library is initialized in the main source file

void sleepHelperConfig();                           // Takes temperature and stores in current

#endif