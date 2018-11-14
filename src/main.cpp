#include <msp430.h> 

#include "stepper/stepper.h"


void testStepperForward();
void testCompass();


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	testStepperForward();
	testCompass();
}
