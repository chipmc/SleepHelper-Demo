//Particle Functions
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
 * D14 - A5 / SPI SS -      
 * D13 - SCK - SPI Clock -  
 * D12 - MO - SPI MOSI -    
 * D11 - MI - SPI MISO -   
 * D10 - UART RX -
 * D9 - UART TX -

 Right Size (12 pins)
 * Li+
 * ENABLE
 * VUSB -
 * D8 -                     Wake Connected to Watchdog Timer
 * D7 -                     Blue Led
 * D6 -                     
 * D5 -                     
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
const pin_t BUTTON_PIN      = D4;
const pin_t BLUE_LED        = D7;
const pin_t WAKEUP_PIN      = D8;
