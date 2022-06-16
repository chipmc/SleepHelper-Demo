//Particle Functions
#include "take_measurements.h"
#include "storage_objects.h"
#include "device_pinout.h"
#include "Particle.h"


/**
 * @brief This code collects temperature data from the TMP-36
 * 
 * @details Uses an analog input and the appropriate scaling
 * 
 * @returns Returns true if succesful and puts the data into the current object
 * 
 */

bool readTempC() {

    digitalWrite(TMP36_POWER_PIN, HIGH);
    delay(2);

    int adcValue = analogRead(TMP36_SENSE_PIN);
    digitalWrite(TMP36_POWER_PIN, LOW);

    // Analog inputs have values from 0-4095, or
    // 12-bit precision. 0 = 0V, 4095 = 3.3V, 0.0008 volts (0.8 mV) per unit
    // The temperature sensor docs use millivolts (mV), so use 3300 as the factor instead of 3.3.
    float mV = ((float)adcValue) * 3300 / 4095;

    // According to the TMP36 docs:
    // Offset voltage 500 mV, scaling 10 mV/deg C, output voltage at 25C = 750 mV (77F)
    // The offset voltage is subtracted from the actual voltage, allowing negative temperatures
    // with positive voltages.

    // Example value=969 mV=780.7 tempC=28.06884765625 tempF=82.52392578125

    // With the TMP36, with the flat side facing you, the pins are:
    // Vcc | Analog Out | Ground
    // You must put a 0.1 uF capacitor between the analog output and ground or you'll get crazy
    // inaccurate values!

    // As configured above, connect VCC to A1 and Analog Out to A0.

    current.tempC = (mV - 500) / 10;

    snprintf(tempString,sizeof(tempString), "%4.2f C", current.tempC);

    return true;
}

