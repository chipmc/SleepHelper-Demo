# SleepHelper-Demo

A demonstration project for the new Sleep Helper library

## Goal of this project

The idea of this project is to demonstrate one way to use the new Sleep Helper library in a way that should be easily updateable and maintainable.

Central to this approach is to reduce the amount of customization of the main source file and break out functionality into header files.  If this approach proved useful, it should be easy to customize this code to reflect the specifics of your implementation (sensors, sample times, hours of operations, etc.)

I am looking for feedback and suggestions on whether this approach makes sense or if there is a better way.

// Revision history
// v0.00 - Started with Rick's Temperature Example - modified slightly for my pinout and Serial not Serial1
// v0.01 - Added the AB1805 Watchdog timer library
// v0.02 - Updated with the example code and update from version 0.0.2 of the library
// v0.03 - Trying to add the ability to send a Particle Webhook - not working
// v0.04 - Changed the structure of the code to make it easier to maintain / more modular
// v0.05 - Moved Sleep Helper configuration into a separate function
// v0.06 - Added persistent storage and particle control using Particle functions
// v0.07 - Incorporated updated libraries, added webhook and fixed bugs

