/**
 * @brief Sleep Helper Demo
 * @details Sleep Helper Demo - Generic implementation with Minimum Functionality For Field Testing.  The idea is to build a strcuture that can be easily maintained and modified
 * without leading to a single huge mainfile.  Much of the code needed for a sleep / wake / measure / report use case is included in the Sleep Helper file.  I am trying 
 * to make sure it is easy to adapt this code to new use cases
 * @author Chip McClelland based on Library and Example Code by @Rickkas
 * @link https://rickkas7.github.io/SleepHelper/index.html @endlink - Documentation
 * @link https://github.com/rickkas7/SleepHelper/ @endlink - Project Repository
 * @date 31 May 2022
 * 
 * To use this file, please consider these steps
 * 1) Open the device pinout header and update with the pinout for your application
 * 2) Open the storage objects header and define what system and current data objects are right for your application
 * 3) Edit the take measurements header and c files to reflect the data you want to collect and put it into the correct objects
 * 4) Edit the sleep helper config file to configure the behaviour of your device
 * 
 */

// Include needed Particle / Community libraries
#include "AB1805_RK.h"
#include "PublishQueuePosixRK.h"
#include "SleepHelper.h"
#include "LocalTimeRK.h"
// Include headers that are part of this program's structure
#include "storage_objects.h"                        // Where we define our structures for storing data
#include "device_pinout.h"                          // Where we store the pinout for our device
#include "take_measurements.h"                      // This is the code that collects data from our particular sensor configuration
#include "particle_fn.h"                            // Place where common Particle functions will go
#include "sleep_helper_config.h"                    // This is where we set the parameters for the Sleep Helper library

// Set logging level and Serial port (USB or Serial1)
SerialLogHandler logHandler(LOG_LEVEL_INFO, {       // Changed to USB log handler from Serial1
	{ "app.pubq", LOG_LEVEL_TRACE },                // Add additional logging for PublishQueuePosixRK
	{ "app.seqfile", LOG_LEVEL_TRACE }              // And the underlying sequential file library used by PublishQueuePosixRK
});                        

// Set the system modes
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP(System.enableFeature(FEATURE_RESET_INFO));  // So we know why the device reset

// Instantiate services and objects
AB1805 ab1805(Wire);                                // Rickkas' RTC / Watchdog library
struct systemStatus_structure sysStatus;            // See structure definition in storage_objects.h
struct current_structure current;                   

// Variables
char tempString[16];

// Support for Particle Products (changes coming in 4.x - https://docs.particle.io/cards/firmware/macros/product_id/)
PRODUCT_ID(PLATFORM_ID);                            // Device needs to be added to product ahead of time.  Remove once we go to deviceOS@4.x
PRODUCT_VERSION(0);
char currentPointRelease[6] ="0.05";


void setup() {

    Particle.variable("tempC", tempString);

    Particle.function("Set Mode", setLowPowerMode);
    Particle.function("Set Wake Time", setWakeTime);
    Particle.function("Set Sleep Time", setSleepTime);

    // Initialize AB1805 Watchdog and RTC
    {
        ab1805.setup();

        // Reset the AB1805 configuration to default values
        ab1805.resetConfig();

        // Enable watchdog
        ab1805.setWDT(AB1805::WATCHDOG_MAX_SECONDS);
    }

    // Initialize PublishQueuePosixRK
	PublishQueuePosix::instance().setup();

    // Configure and initialize the Sleep Helper function
    sleepHelperConfig();                                 // This is the function call to configure the sleep helper parameters

    SleepHelper::instance().setup();                    // This puts these parameters into action
}

void loop() {
    SleepHelper::instance().loop();

    ab1805.loop();
    
    PublishQueuePosix::instance().loop();
}
