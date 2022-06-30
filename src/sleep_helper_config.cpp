//Particle Functions
#include "Particle.h"
#include "sleep_helper_config.h"

// Battery conect information - https://docs.particle.io/reference/device-os/firmware/boron/#batterystate-
const char* batteryContext[7] = {"Unknown","Not Charging","Charging","Charged","Discharging","Fault","Diconnected"};

void sleepHelperConfig() {

    SleepHelper::instance()
        .withMinimumCellularOffTime(5min)                                                           // 
        .withMaximumTimeToConnect(11min)
        .withTimeConfig("EST5EDT,M3.2.0/02:00:00,M11.1.0/02:00:00")
        .withEventHistory("/usr/events.txt", "eh")
        .withDataCaptureFunction([](SleepHelper::AppCallbackState &state) {
            if (Time.isValid()) {
                batteryState();
                isItSafeToCharge();
                readTempC();
                SleepHelper::instance().addEvent([](JSONWriter &writer) {
                    writer.name("t").value((int) Time.now());
                    writer.name("bs").value(current.batteryState);
                    writer.name("c").value(current.tempC);
                });
            }
            return false;
        })
        .withSleepConfigurationFunction([](SystemSleepConfiguration &sleepConfig, SleepHelper::SleepConfigurationParameters &params) {
            // Add a GPIO wake on button press
            sleepConfig.gpio(BUTTON_PIN, CHANGE);   // My debounce time constant prevents detecting FALLING
            delay(2000);                            // This is a debugging line - to connect to USB serial for logging
            Log.info("Woke on button press");
            if (!digitalRead(BUTTON_PIN)) {         // The BUTTON is active low - this is a button press
                sysStatus.enableSleep = false;      // Pressing the button diables sleep - at least that is the intent
                Log.info("Button press - sleep enable is %s", (sysStatus.enableSleep) ? "true" : "false");
            }
            return true;
        })
        .withSleepReadyFunction([](SleepHelper::AppCallbackState &, system_tick_t) {
            if (sysStatus.enableSleep) return false;// Boolean set by Particle.function - If sleep is enabled return false
            else return true;                       // If we need to delay sleep, return true
        })
        .withAB1805_WDT(ab1805)                     // Stop the watchdog before sleep or reset, and resume after wake
        .withPublishQueuePosixRK()                  // Manage both internal publish queueing and PublishQueuePosixRK
        ;

    // Full wake and publish
    // Every 15 minutes from 9:00 AM to 10:00 PM local time on weekdays (not Saturday or Sunday)
    // Every 2 hours other times
    SleepHelper::instance().getScheduleFull()
        .withMinuteOfHour(15, LocalTimeRange(LocalTimeHMS("09:00:00"), LocalTimeHMS("21:59:59"), LocalTimeRestrictedDate(LocalTimeDayOfWeek::MASK_WEEKDAY)))
        .withHourOfDay(2);

    // Data capture every 5 minutes
    SleepHelper::instance().getScheduleDataCapture()
        .withMinuteOfHour(5);
}
