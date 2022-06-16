#include "SleepHelper.h"

SerialLogHandler logHandler;

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {
    // Uncomment this to wait for a serial connection for 10 seconds so you can see the debug log messages
    // waitFor(Serial.isConnected, 10000);
    // delay(2000);

    SleepHelper::instance()
        .withShouldConnectMinimumSoC(9.0)
        .withMaximumTimeToConnect(11min);

    SleepHelper::instance().withSetupFunction([]() {
        Log.info("test setup!");
        return true;
    });

    SleepHelper::instance().setup();
}

void loop() {
    SleepHelper::instance().loop();
}

