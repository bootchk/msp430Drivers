#pragma once

#include <cstdint>

/*
PWM pulse width modulation.
Generates a train of pulses wth given duty cycle.
Pulses are squarish.

Uses a TIMER_A device.
The device operates concurrently with the MCU.

Here the API is just to turn the train on and off.
The caller must time the length of the train,
often by sleeping.

Drives a pin.
The pin is hardcoded by your choice from MSP430 family.
Hardcodes choice of TimerA number 0 (TA0).
For some family members,
having more instances of TimerA device,
you could change code to use other instance and pin.
*/
class PWM {
    public:
    static void turnOn(uint16_t dutyCycle);
	static void turnOff();
};
