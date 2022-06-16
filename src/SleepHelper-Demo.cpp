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
 * 
 */

// Revision history
// v0.00 - Started with Rick's Temperature Example - modified slightly for my pinout and Serial not Serial1
// v0.01 - Added the AB1805 Watchdog timer library
// v0.02 - Updated with the example code and update from version 0.0.2 of the library
// v0.03 - Trying to add the ability to send a Particle Webhook - not working
// v0.04 - Changed the structure of the code to make it easier to maintain / more modular

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
char currentPointRelease[6] ="0.04";


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

    SleepHelper::instance()
        .withMinimumCellularOffTime(5min)
        .withMaximumTimeToConnect(11min)
        .withTimeConfig("EST5EDT,M3.2.0/02:00:00,M11.1.0/02:00:00")
        .withEventHistory("/usr/events.txt", "eh")
        .withDataCaptureFunction([](SleepHelper::AppCallbackState &state) {
            if (Time.isValid()) {
                SleepHelper::instance().addEvent([](JSONWriter &writer) {
                    writer.name("t").value((int) Time.now());
                    writer.name("c").value(readTempC(), 1);
                });
                char dataStr[16];           // While we are here see if I can send a webhook to the queue
                snprintf(dataStr,sizeof(dataStr),"t: %4.2f",current.tempC);
                PublishQueuePosix::instance().publish("Test", dataStr, PRIVATE);
                Log.info(dataStr);          // Visibility to the payload in the webhook
            }
            return false;
        })
        .withWakeFunction([](const SystemSleepResult &sleepResult) {
            delay(2000);                       // Delay so we can capture in serial monitor
            return true;
        })
        .withAB1805_WDT(ab1805)                // Stop the watchdog before sleep or reset, and resume after wake
        .withPublishQueuePosixRK()             // Manage both internal publish queueing and PublishQueuePosixRK
        ;

    // Full wake and publish
    // - Every 15 minutes from 9:00 AM to 5:00 PM local time on weekdays (not Saturday or Sunday)
    // - Every 2 hours other times
    SleepHelper::instance().getScheduleFull()
        .withMinuteOfHour(15, LocalTimeRange(LocalTimeHMS("09:00:00"), LocalTimeHMS("16:59:59"), LocalTimeRestrictedDate(LocalTimeDayOfWeek::MASK_WEEKDAY)))
        .withHourOfDay(2);

    // Data capture every 2 minutes
    SleepHelper::instance().getScheduleDataCapture()
        .withMinuteOfHour(2);

    SleepHelper::instance().setup();
}

void loop() {
    SleepHelper::instance().loop();

    ab1805.loop();
    PublishQueuePosix::instance().loop();
}
