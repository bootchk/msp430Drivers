#include <msp430.h> 

#include "stepper/stepper.h"




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	Stepper::reset();
	// assert GPIO configured

	// TODO unlock lpm5

	// forever, single step forward
	while (true) {
	    Stepper::singleStepForward();

	    __delay_cycles(500000);
	}
}
