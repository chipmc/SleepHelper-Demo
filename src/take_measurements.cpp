//Particle Functions
#include "Particle.h"
#include "take_measurements.h"

FuelGauge fuelGauge;                                // Needed to address issue with updates in low battery state 

char tempString[16] = " ";                          // Initialized here - external everywhere else

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

    Log.info("Temperature is %s",tempString);

    return true;
}


/**
 * @brief In this function, we will measure the battery state of charge and the current functional state
 * 
 * @details One factor that is an issue today is the accurace of the state of charge if the device is waking
 * from sleep.  In order to help with this, there is a test for enable sleep and an additional delay.
 * 
 * @return true  - If the battery has a charge over 60%
 * @return false - Less than 60% indicates a low battery condition
 */
bool batteryState() {
    current.batteryState = System.batteryState();                      // Call before isItSafeToCharge() as it may overwrite the context

  if (sysStatus.enableSleep) {                                        // Need to take these steps if we are sleeping
    fuelGauge.quickStart();                                            // May help us re-establish a baseline for SoC
    delay(500);
  }

  current.stateOfCharge = int(fuelGauge.getSoC());                   // Assign to system value

  if (current.stateOfCharge > 60) return true;
  else return false;
}

/**
 * @brief Checks to see if the temperature is in the range to support charging
 * 
 * @details Will enable or disable charging based on the current temperature
 * 
 * @link https://batteryuniversity.com/learn/article/charging_at_high_and_low_temperatures @endlink
 * 
 */
bool isItSafeToCharge()                             // Returns a true or false if the battery is in a safe charging range.
{
  PMIC pmic(true);
  if (current.tempC < 0 || current.tempC > 37 )  {  // Reference: (32 to 113 but with safety)
    pmic.disableCharging();                         // It is too cold or too hot to safely charge the battery
    current.batteryState = 1;                       // Overwrites the values from the batteryState API to reflect that we are "Not Charging"
    return false;
  }
  else {
    pmic.enableCharging();                          // It is safe to charge the battery
    return true;
  }
}