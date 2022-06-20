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
    currentCountsAddr     = 0x50                    // Where we store the current counts data structure
  };
}

const int FRAMversionNumber = 1;

extern MB85RC64 fram; 
struct systemStatus_structure sysStatus;            // See structure definition in storage_objects.h
struct current_structure current;     

bool initializeStorage() {
    // Next we will load FRAM and check or reset variables to their correct values
  fram.begin();                                                        // Initialize the FRAM module
  byte tempVersion;
  fram.get(FRAM::versionAddr, tempVersion);                            // Load the FRAM memory map version into a variable for comparison
  if (tempVersion != FRAMversionNumber) {                              // Check to see if the memory map in the sketch matches the data on the chip
    fram.erase();                                                      // Reset the FRAM to correct the issue
    fram.put(FRAM::versionAddr, FRAMversionNumber);                    // Put the right value in
    fram.get(FRAM::versionAddr, tempVersion);                          // See if this worked
    if (tempVersion != FRAMversionNumber) {
      // state = ERROR_STATE;                                             // Device will not work without FRAM will need to reset
      // resetTimeStamp = millis();                                       // Likely close to zero but, for form's sake
      // current.alerts = 12;                                             // FRAM is messed up so can't store but will be read in ERROR state
    }
    // else loadSystemDefaults();                                         // Out of the box, we need the device to be awake and connected
  }
  else {
    fram.get(FRAM::systemStatusAddr,sysStatus);                        // Loads the System Status array from FRAM
    fram.get(FRAM::currentCountsAddr,current);                         // Loead the current values array from FRAM
  }

  return true;
}