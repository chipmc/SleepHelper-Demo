//Particle Functions
#include "AB1805_RK.h"
#include "PublishQueuePosixRK.h"
#include "SleepHelper.h"
#include "take_measurements.h"
#include "storage_objects.h"
#include "device_pinout.h"
#include "Particle.h"

extern AB1805 ab1805;                                // Rickkas' RTC / Watchdog library

system_tick_t pinWakeMillis = 0;
system_tick_t lastButtonPress = 0;

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
                SleepHelper::instance().addEvent([](JSONWriter &writer) {
                    writer.name("t").value((int) Time.now());
                    writer.name("c").value(readTempC(), 1);
                });
                batteryState();
                isItSafeToCharge();
                char dataStr[128];           // Sends a Webhook based on these readings
                snprintf(dataStr, sizeof(dataStr), "{\"battery\":%i,\"key1\":\"%s\",\"temp\":%4.2f, \"timestamp\":%lu000}", current.stateOfCharge, batteryContext[current.batteryState], current.tempC, Time.now());
                PublishQueuePosix::instance().publish("Sleepy-Use-Case-Webhook", dataStr, PRIVATE);
                Log.info(dataStr);          // Visibility to the payload in the webhook
            }
            return false;
        })
        .withSleepConfigurationFunction([](SystemSleepConfiguration &sleepConfig, SleepHelper::SleepConfigurationParameters &params) {
            // Add a GPIO wake on button press
            sleepConfig.gpio(BUTTON_PIN, CHANGE);
            if (!digitalRead(BUTTON_PIN)) sysStatus.enableSleep = false;
            return true;
        })
        .withWakeFunction([](const SystemSleepResult &sleepResult) {
            if (sleepResult.wakeupReason() == SystemSleepWakeupReason::BY_GPIO) {
                pin_t whichPin = sleepResult.wakeupPin();
                Log.info("wake by pin %d", whichPin);
                if (whichPin == BUTTON_PIN) {
                    lastButtonPress = pinWakeMillis = millis();
                }
                else {
                    pinWakeMillis = 0;
                }
            }
            return true;
        })
        .withShouldConnectFunction([](int &connectConviction, int &noConnectConviction) {
            if (pinWakeMillis) {
                // Waking by the user pressing the button signals the need to connect.
                noConnectConviction = 0;
            }
            return true;
        })
        .withNoConnectionFunction([](SleepHelper::AppCallbackState &state) {
            // If woken by pin, wait until button is released
            if (pinWakeMillis) {
                // return true to stay awake, false to allow sleep
                return (digitalRead(BUTTON_PIN) == LOW);
            }
            else {
                return false;
            }
        })
        .withSleepReadyFunction([](SleepHelper::AppCallbackState &, system_tick_t) {
            if (sysStatus.enableSleep) return false;      // Boolean set by Particle.function - If sleep is enabled return false
            else return true;                             // If we need to delay sleep, return true
        })
        .withAB1805_WDT(ab1805)                // Stop the watchdog before sleep or reset, and resume after wake
        .withPublishQueuePosixRK()             // Manage both internal publish queueing and PublishQueuePosixRK
        ;

    // Full wake and publish
    // Every 15 minutes from 9:00 AM to 10:00 PM local time on weekdays (not Saturday or Sunday)
    // Every 2 hours other times
    SleepHelper::instance().getScheduleFull()
        .withMinuteOfHour(15, LocalTimeRange(LocalTimeHMS("09:00:00"), LocalTimeHMS("21:59:59"), LocalTimeRestrictedDate(LocalTimeDayOfWeek::MASK_WEEKDAY)))
        .withHourOfDay(2);

    // Data capture every 2 minutes
    SleepHelper::instance().getScheduleDataCapture()
        .withMinuteOfHour(2);
}
