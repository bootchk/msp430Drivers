#include <msp430.h> 

#include "stepper.h"




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	Stepper::reset();
	Stepper::configureIOPortForMotorControl();

	// TODO unlock lpm5

	// forever, single step forward
	while (true) {
	    Stepper::singleStepForward();
	}
}
