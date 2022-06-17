//Particle Functions
#include "AB1805_RK.h"
#include "PublishQueuePosixRK.h"
#include "SleepHelper.h"
#include "take_measurements.h"
#include "storage_objects.h"
#include "device_pinout.h"
#include "Particle.h"

extern AB1805 ab1805;                                // Rickkas' RTC / Watchdog library

void sleepHelperConfig() {

    SleepHelper::instance()
        .withMinimumCellularOffTime(5min)                                                           // 
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
}
