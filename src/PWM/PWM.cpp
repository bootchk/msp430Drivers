
#include "PWM.h"

// depends on register symbols
#include <msp430.h>

// device abstraction: MSPWare DriverLib
// Implementation uses DriverLib
// depends on msp430.h
#include <timer_a.h>

// logical device abstraction: msp430drivers
#include "../pinFunction/pwmPins.h"


// Declare timer peripheral to use
#define TIMER_BASE_ADDRESS TIMER_A0_BASE

/*
The PWM generates pulses at the clock freq (32khz)
divided by the timer period (100).
That is 320Hz.
For a higher freq, use a faster clock.
*/


void PWM::turnOff() {
	Timer_A_stop(TIMER_BASE_ADDRESS);

	// Stopping timer leaves the output indeterminate value, high or low.
	// Ensure it is low.
	PWMPins::configureToDigitalLow();
}



// turn on a train of PWM  pulses on a pin.
// Remains on until you turn it off.
// Requires ACLK is on.
// Configures the GPIO pin speced in board.h.
// Must be the same pin as the MSP430 model routes to
// instance of TimerA specified above (TIMER_BASE_ADDRESS)
// Must be the same pin as the MSP430 model routes to
// instance of TimerA specified above (TIMER_A0_BASE, or 1,2,3
// See data sheet for model of family.

void PWM::turnOn(uint16_t dutyCycle) {
	// Init param struct to zero
	Timer_A_outputPWMParam config = {0};

	// Fill fields of param struct. Order not matter
	
	// Arbitrarily choose clock source is aux clock ACLCK
	// ACLCK defaults to on with source REFO at 32khz
	config.clockSource        = TIMER_A_CLOCKSOURCE_ACLK;
	config.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;

	// CCR0 is not externally connected on some models of family
	// so use CCR1.
	config.compareRegister    = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	// Output set on at CCR0 (period) and reset off at CCR1 (duty cycle)
	config.compareOutputMode  = TIMER_A_OUTPUTMODE_RESET_SET;

	// Arbitrary timer period of 100
	// dutyCycle in [1,100] ? e.g. 50 yields 50% duty cycle
	config.timerPeriod        = 100;
	config.dutyCycle          = dutyCycle;

	// Configure GPIO pin.
	// You can move this to main.
	PWMPins::configure();
	
	// Configure and start a TimerA peripheral.
	// Using DriverLib
	Timer_A_outputPWM(TIMER_BASE_ADDRESS, &config);

	// The above started PWM pulses, no separate start is required.
	
	// assert timer is started and generating PWM on the configured pin
}

