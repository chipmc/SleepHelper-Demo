/* 
* This file contains the storage objects for the system and the current values.  
* Defining these as objects allows us to store them in persistent storage
*
* To dos: 
* Figure out some way to abstract the storage method (EEPROM, Flash, FRAM)
* Make a more comprehensive set of system varaibles as changing this object could cause upgrade issues in future releases
*/


#ifndef SYS_STATUS_H
#define SYS_STATUS_H

#include "MB85RC256V-FRAM-RK.h"                     // Include this library if you are using FRAM
#include "Particle.h"

extern const int FRAMversionNumber;                    // Increment this number each time the memory map is changed

struct systemStatus_structure {                     // Where we store the configuration / status of the device
  uint8_t structuresVersion;                        // Version of the data structures (system and current)
  int currentConnectionLimit;                       // Here we will store the connection limit in seconds
  bool verboseMode;                                 // Turns on extra messaging
  bool solarPowerMode;                              // Powered by a solar panel or utility power
  bool enableSleep;                                // Low Power Mode will disconnect from the Cellular network to save power
  uint8_t wakeTime;                                 // Hour to start operations (0-23)
  uint8_t sleepTime;                                // Hour to go to sleep for the night (0-23)
};

extern struct systemStatus_structure sysStatus;

struct current_structure {                          // Where we store values in the current wake cycle
  float tempC;                                      // Current temperature in degrees C
  int stateOfCharge;                                // Battery charge level
  uint8_t batteryState;                             // Stores the current battery state (charging, discharging, etc)
  time_t lastCountTime;                             // Timestamp of last data collection
  uint16_t lastConnectionDuration;                  // How long - in seconds - did it take to last connect to the Particle cloud
};

extern struct current_structure current;

bool initializeStorage();                           // Initialize the storage instance
bool storeObjects();                                // Store the current and sysStatus objects

#endif
