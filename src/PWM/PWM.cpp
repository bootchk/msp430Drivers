
#include "PWM.h"

// depends on register symbols
#include <msp430.h>

#include <board.h>

// device abstraction: MSPWare DriverLib
// Implementation uses DriverLib
// depends on msp430.h
#include <timer_a.h>
#include <timer_b.h>
 
// logical device abstraction: msp430drivers
#include "../pinFunction/pwmPins.h"

#include "../assert/myAssert.h"

/* 
This is confusing re TimerA vs TimerB.
I suppose because originally coded for TimerA,
and not all MSP430 instances have TimerB,
and DriverLib does not abstract the difference away?
See board.h which aliases TimerA as TimerB.
*/

/*
The PWM generates pulses at the clock freq divided by the timer period (100).
For aux clock at 32kHz, pulse frequency is 320Hz.
For a higher freq, use a faster clock.
*/


void PWM::turnOff() {
	Timer_A_stop(PWM_BASE_ADDRESS);

	// Stopping timer leaves the output indeterminate value, high or low.
	// Ensure it is low.
	PWMPins::configureToDigitalLow();

	// PWM network has a pulldown.
	// When enter LPMx.5 and pins are high impedance, PWM net remains low.
	// When we wake from LPMx.5, MCU also drives PWM net low.
}

/* 
The somewhat arbitrary period of the PWM.
Does not need to be a power of two.
The greater the value, the less the frequency of the PWM.

The consumer of the PWM accepts a certain range of frequencies.
E.G. a motor driver IC on its PWM pin.
With 1Mhz SMCLOCK and divider 1 and period 64
yields 15.6kHz freq on PWM.
DRV10866 accepts 15-50kHz
!!! A period greater than that would yield
a PWM frequency lower than the driver IC requires.
*/
#define PWM_PERIOD 64

/* 
Return a value in units TimerTicks
that represents dutyCycle in %.
To set as the value of a compare register.
*/
static uint16_t
timerTicksInDutyCycle (uint16_t dutyCycle)
{
	// Integer math, so fast but rounded.
	// 100 is the denominator that converts
	// an integer percent duty cycle to a fraction.
	return dutyCycle * PWM_PERIOD / 100;
}


// Uses DriverLib
static void
setTimerForPWM(uint16_t dutyCycle)
{
	// Init param struct to zero
	Timer_A_outputPWMParam config = {0};

	// Fill fields of param struct. Order not matter
	
	// Arbitrarily choose clock source is aux clock ACLCK
	// ACLCK defaults to on with source REFO at 32khz
	// config.clockSource        = TIMER_A_CLOCKSOURCE_ACLK;
	// config.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;

	// SMCLK @ 1mHz / 100 period yields 10k
	config.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;
	config.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;

	// CCR0 is not externally connected on some models of family
	// so use CCR1.
	config.compareRegister    = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	// Output set on at CCR0 (period) and reset off at CCR1 (duty cycle)
	config.compareOutputMode  = TIMER_A_OUTPUTMODE_RESET_SET;

	// Arbitrary timer period of 100
	// dutyCycle in [1,100] ? e.g. 50 yields 50% duty cycle
	// config.timerPeriod        = 100;
	
	config.timerPeriod        = PWM_PERIOD;
	config.dutyCycle          = timerTicksInDutyCycle(dutyCycle);

	// Configure GPIO pin.
	// You can move this to main.
	PWMPins::configure();
	
	// Configure and start a Timer peripheral to generate PWM.
	// Using DriverLib
	Timer_A_outputPWM(PWM_BASE_ADDRESS, &config);

	// Docs say PWM starts without need this:
	//Timer_A_startCounter(TIMER_BASE_ADDRESS, TIMER_A_UPDOWN_MODE);
	
	// Timer count mode is not STOP
	myAssert(* (int*) (PWM_BASE_ADDRESS + OFS_TAxCTL) & TIMER_A_UPDOWN_MODE);
};




/*
turn on a train of PWM  pulses on a pin.
Remains on until you turn it off.

Requires clock (ACLK or SMCLK) is on.

Configures the GPIO pin speced in board.h.
Must be the same pin as the MSP430 model routes to
instance of TimerA specified above (TIMER_A0_BASE, or 1,2,3
See data sheet for model of family.

dutyCycle is a percent in range [0,100]
*/

void 
PWM::turnOn(uint16_t dutyCycle)
{
	if (dutyCycle == 100)
	{
		PWMPins::configureToDigitalHigh();
	}
	else 
	{
		setTimerForPWM(dutyCycle);
		// The above started PWM pulses, no separate start is required.
	}
	
	// assert timer is started and generating PWM on the configured pin
	// !!! Or the pin is high, for 100% duty cycle
}


/*
Set duty cycle while PWM in progress.
On some Timer devices, may yield abberant pulse.
On TimerB device, register is synced, so no abberant pulse.

DriverLib has no support for this.

Change the CCR1 register which is the duty cycle.
*/
void 
PWM::setDutyCycleDynamically(uint16_t dutyCycle)
{
	/*
	Require dutyCycle < 100.
	Expect timer is generating PWM, but has no effect if not.
	*/

	// Convert to TimerTicks relative to the period (in CCR0)
	// Store in CCR1
	HWREG16(PWM_BASE_ADDRESS + OFS_TBxCCR1) = timerTicksInDutyCycle(dutyCycle);
}