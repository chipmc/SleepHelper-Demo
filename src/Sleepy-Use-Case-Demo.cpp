/**
 * @brief Sleep Helper Demo
 * @details Sleep Helper Demo - Generic implementation with Minimum Functionality For Field Testing.  The idea is to build a strcuture that can be easily maintained and modified
 * without leading to a single huge mainfile.  I am trying to make sure it is easy to adapt this code to new use cases
 * @author Chip McClelland based on Library and Example Code by @Rickkas
 * @link https://rickkas7.github.io/SleepHelper/index.html @endlink - Documentation
 * @link https://github.com/rickkas7/SleepHelper/ @endlink - Project Repository
 * @date 31 May 2022
 * 
 * To keep the main application manageable, I have broken out configuration into focused files
 * 1) device_pinout - Pin definitions and connections for your device
 * 2) storage_object - Define what variables are needed to capture the sustem and current status
 * 3) take_measurements - This is the set of activities executed each time the device wakes
 * 4) sleep_helper_config - Define the sleep / wake / report cycle - the full behaviour of your device
 * 5) particle_fn - For any particle variables / functions that you want to expose in the console
 */

// Include needed Particle / Community libraries
#include "Particle.h"
#include "AB1805_RK.h"
#include "MB85RC256V-FRAM-RK.h"
#include "PublishQueuePosixRK.h"
#include "SleepHelper.h"

// Include headers that are part of this program's structure and called in this source file
#include "particle_fn.h"                            // Place where common Particle functions will go
#include "sleep_helper_config.h"                    // This is where we set the parameters for the Sleep Helper library

// Set logging level and Serial port (USB or Serial1)
SerialLogHandler logHandler(LOG_LEVEL_INFO);       //  Limit logging to information on program flow               

// Set the system modes
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP(System.enableFeature(FEATURE_RESET_INFO));  // So we know why the device reset

// Instantiate services and objects - all other references need to be external
AB1805 ab1805(Wire);                                // Rickkas' RTC / Watchdog library
MB85RC64 fram(Wire, 0);                             // Rickkas' FRAM library

// Support for Particle Products (changes coming in 4.x - https://docs.particle.io/cards/firmware/macros/product_id/)
PRODUCT_ID(PLATFORM_ID);                            // Device needs to be added to product ahead of time.  Remove once we go to deviceOS@4.x
PRODUCT_VERSION(0);
char currentPointRelease[6] ="0.09";

void setup() {

    initializePinModes();                           // Sets the pinModes

    particleInitialize();                           // Sets up all the Particle functions and variables defined in particle_fn.h

    {                                               // Initialize AB1805 Watchdog and RTC                                 
        ab1805.setup();

        ab1805.resetConfig();                       // Reset the AB1805 configuration to default values

        ab1805.setWDT(AB1805::WATCHDOG_MAX_SECONDS);// Enable watchdog
    }

	PublishQueuePosix::instance().setup();          // Initialize PublishQueuePosixRK

    storageObjectStart();                           // Sets up the storage for system and current status in storage_objects.h

    sleepHelperConfig();                            // This is the function call to configure the sleep helper parameters

    SleepHelper::instance().setup();                // This puts these parameters into action
}

void loop() {
    SleepHelper::instance().loop();                 // Monitor and manage the sleep helper workflow

    ab1805.loop();                                  // Monitor the real time clock and watchdog
    
    PublishQueuePosix::instance().loop();           // Monitor and manage the publish queue

    storageObjectLoop();                            // Compares current system and current objects and stores if the hash changes (once / second)
}
