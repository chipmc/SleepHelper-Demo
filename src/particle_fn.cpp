//Particle Functions
#include "Particle.h"
#include "particle_fn.h"

/**
 * @brief Returns an integer to support Particle variable limitations
 * 
 */
int convertWakeToInt() {
  int returnValue;
  returnValue = sysStatus.wakeTime;
  return returnValue;
}
int convertSleepToInt() {
  return (int)sysStatus.sleepTime;
}

/**
 * @brief Initializes the Particle functions and variables
 * 
 * @details If new particles of functions are defined, they need to be initialized here
 * 
 */
void particleInitialize() {
  Log.info("Initializing Particle functions and variables");
  Particle.variable("tempC", tempString);
  Particle.variable("Wake Time", convertWakeToInt);
  Particle.variable("Sleep Time", convertSleepToInt);
  Particle.variable("Sleep Enabled",(sysStatus.enableSleep) ? "Yes" : "No");
  Particle.variable("Release",currentPointRelease);

  Particle.function("Enable Sleep", setEnableSleep);
  Particle.function("Set Wake Time", setWakeTime);
  Particle.function("Set Sleep Time", setSleepTime);
}

/**
 * @brief Sets the closing time of the facility.
 *
 * @details Extracts the integer from the string passed in, and sets the closing time of the facility
 * based on this input. Fails if the input is invalid.
 *
 * @param command A string indicating what the closing hour of the facility is in 24-hour time.
 * Inputs outside of "0" - "24" will cause the function to return 0 to indicate an invalid entry.
 *
 * @return 1 if able to successfully take action, 0 if invalid command
 */
int setWakeTime(String command)
{
  char * pEND;
  char data[64];
  int tempTime = strtol(command,&pEND,10);                             // Looks for the first integer and interprets it
  if ((tempTime < 0) || (tempTime > 23)) return 0;                     // Make sure it falls in a valid range or send a "fail" result
  sysStatus.wakeTime = tempTime;
  snprintf(data, sizeof(data), "Open time set to %i",sysStatus.wakeTime);
  Log.info(data);
  if (Particle.connected()) {
    Particle.publish("Time",data, PRIVATE);
  }
  return 1;
}

/**
 * @brief Sets the closing time of the facility.
 *
 * @details Extracts the integer from the string passed in, and sets the closing time of the facility
 * based on this input. Fails if the input is invalid.
 *
 * @param command A string indicating what the closing hour of the facility is in 24-hour time.
 * Inputs outside of "0" - "24" will cause the function to return 0 to indicate an invalid entry.
 *
 * @return 1 if able to successfully take action, 0 if invalid command
 */
int setSleepTime(String command)
{
  char * pEND;
  char data[64];
  int tempTime = strtol(command,&pEND,10);                       // Looks for the first integer and interprets it
  if ((tempTime < 0) || (tempTime > 24)) return 0;   // Make sure it falls in a valid range or send a "fail" result
  sysStatus.sleepTime = tempTime;
  snprintf(data, sizeof(data), "Closing time set to %i",sysStatus.sleepTime);
  Log.info(data);
  if (Particle.connected()) {
    Particle.publish("Time",data, PRIVATE);
  }
  return 1;
}

/**
 * @brief Toggles the device into low power mode based on the input command.
 *
 * @details If the command is "1", sets the device into low power mode. If the command is "0",
 * sets the device into normal mode. Fails if neither of these are the inputs.
 *
 * @param command A string indicating whether to set the device into low power mode or into normal mode.
 * A "1" indicates low power mode, a "0" indicates normal mode. Inputs that are neither of these commands
 * will cause the function to return 0 to indicate an invalid entry.
 *
 * @return 1 if able to successfully take action, 0 if invalid command
 */
int setEnableSleep(String command)                                   // This is where we can put the device into low power mode if needed
{
  char data[64];
  if (command != "1" && command != "0") return 0;                     // Before we begin, let's make sure we have a valid input
  if (command == "1") {                                               // Command calls for enabling sleep
    sysStatus.enableSleep = true;
  }
  else {                                                             // Command calls for disabling sleep
    sysStatus.enableSleep = false;
  }
  snprintf(data, sizeof(data), "Enable sleep is %s", (sysStatus.enableSleep) ? "true" : "false");
  Log.info(data);
  if (Particle.connected()) {
    Particle.publish("Mode",data, PRIVATE);
  }
  return 1;
}