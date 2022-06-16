//Particle Functions
#include "take_measurements.h"
#include "storage_objects.h"
#include "device_pinout.h"
#include "Particle.h"

/**********************************************************************************************************************
 * ******************************         Boron Pinout Example               ******************************************
 * https://docs.particle.io/reference/datasheets/b-series/boron-datasheet/#pins-and-button-definitions
 *
 * Left Side (16 pins)
 * !RESET -
 * 3.3V -
 * !MODE -
 * GND -
 * D19 - A0 -
 * D18 - A1 -
 * D17 - A2 -
 * D16 - A3 -
 * D15 - A4 -               TMP32 Temp Sensor
 * D14 - A5 / SPI SS -      disableModule
 * D13 - SCK - SPI Clock -  intPin
 * D12 - MO - SPI MOSI -    
 * D11 - MI - SPI MISO -    ledPower
 * D10 - UART RX -
 * D9 - UART TX -

 Right Size (12 pins)
 * Li+
 * ENABLE
 * VUSB -
 * D8 -                     Wake Connected to Watchdog Timer
 * D7 -                     Blue Led
 * D6 -                     DEEP-SLEEP Enable Pin - Brings Enable Pin low - Only RTC Alarm interrupt will wake
 * D5 -                     Done Pin Connected to the Watchdog Timer
 * D4 -                     User Switch
 * D3 - 
 * D2 - 
 * D1 - SCL - I2C Clock -   FRAM / RTC and I2C Bus
 * D0 - SDA - I2C Data -    FRAM / RTX and I2C Bus
 *
 *
***********************************************************************************************************************/


const pin_t TMP36_SENSE_PIN = A4;
const pin_t TMP36_POWER_PIN = A1;
const pin_t WAKEUP_PIN      = D8;
