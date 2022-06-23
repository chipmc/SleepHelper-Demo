/* 
* This file contains the storage objects for the system and the current values.  
* Defining these as objects allows us to store them in persistent storage
*
* To dos: 
* Figure out some way to abstract the storage method (EEPROM, Flash, FRAM)
* Make a more comprehensive set of system varaibles as changing this object could cause upgrade issues in future releases
*/

#include "MB85RC256V-FRAM-RK.h"                     // Include this library if you are using FRAM
#include "storage_objects.h"
#include "Particle.h"

namespace FRAM {                                    // Moved to namespace instead of #define to limit scope
  enum Addresses {
    versionAddr           = 0x00,                   // Version of the FRAM memory map
    systemStatusAddr      = 0x01,                   // Where we store the system status data structure
    currentStatusAddr     = 0x50                    // Where we store the current counts data structure
  };
}

const int FRAMversionNumber = 1;

extern MB85RC64 fram; 
struct systemStatus_structure sysStatus;            // See structure definition in storage_objects.h
struct current_structure current;     

bool storageObjectStart() {
    // Next we will load FRAM and check or reset variables to their correct values
  fram.begin();                                     // Initialize the FRAM module
  byte tempVersion;
  fram.get(FRAM::versionAddr, tempVersion);         // Load the FRAM memory map version into a variable for comparison
  if (tempVersion != FRAMversionNumber) {           // Check to see if the memory map in the sketch matches the data on the chip
    fram.erase();                                   // Reset the FRAM to correct the issue
    fram.put(FRAM::versionAddr, FRAMversionNumber); // Put the right value in
    fram.get(FRAM::versionAddr, tempVersion);       // See if this worked
    if (tempVersion != FRAMversionNumber) {
      // Need to add an error handler here as the device will not work without FRAM will need to reset
    }
    loadSystemDefaults();                           // Out of the box, we need the device to be awake and connected
  }
  else {
    fram.get(FRAM::systemStatusAddr,sysStatus);     // Loads the System Status array from FRAM
    fram.get(FRAM::currentStatusAddr,current);      // Loead the current values array from FRAM
  }

  return true;
}

bool storageObjectLoop() {                          // Monitors the values of the two objects and writes to FRAM if changed after a second
  static time_t lastCheckMillis = 0;
  static size_t lastSysStatusHash;
  static size_t lastCurrentHash;
  bool returnValue = false;

  if (millis() - lastCheckMillis > 1000) {          // Check once a second
    lastCheckMillis = millis();                     // Limit all this math to once a second
    size_t sysStatusHash =  std::hash<byte>{}(sysStatus.structuresVersion) + \
                      std::hash<int>{}(sysStatus.currentConnectionLimit)+ \
                      std::hash<bool>{}(sysStatus.verboseMode) + \
                      std::hash<bool>{}(sysStatus.solarPowerMode) + \
                      std::hash<bool>{}(sysStatus.enableSleep) + \
                      std::hash<byte>{}(sysStatus.wakeTime) + \
                      std::hash<byte>{}(sysStatus.sleepTime);
    if (sysStatusHash != lastSysStatusHash) {       // If hashes don't match write to FRAM
      Log.info("system Object stored and hash updated");
      fram.put(FRAM::systemStatusAddr,sysStatus);
      lastSysStatusHash = sysStatusHash;
      returnValue = true;                           // In case I want to test whether values changed
    } 
    size_t currentHash =  std::hash<double>{}(current.tempC) + \
                      std::hash<int>{}(current.stateOfCharge)+ \
                      std::hash<byte>{}(current.batteryState) + \
                      std::hash<time_t>{}(current.lastCountTime) + \
                      std::hash<u_int16_t>{}(current.lastConnectionDuration);
    if (currentHash != lastCurrentHash) {           // If hashes don't match write to FRAM
      Log.info("current Object stored and hash updated");
      fram.put(FRAM::currentStatusAddr,current);
      lastCurrentHash = currentHash;
      returnValue = true;
    } 

  }
  return returnValue;
}

void loadSystemDefaults() {
    if (Particle.connected()) {
    Particle.publish("Mode","Loading System Defaults", PRIVATE);
  }
  Log.info("Loading system defaults");
  sysStatus.structuresVersion = 1;
  sysStatus.currentConnectionLimit = 10;
  sysStatus.verboseMode = false;
  sysStatus.solarPowerMode = true;
  sysStatus.enableSleep = true;
  sysStatus.wakeTime = 6;
  sysStatus.sleepTime = 22;
}