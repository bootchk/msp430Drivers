#include <msp430.h> 

#include "stepper/stepper.h"


void testStepper();
void testCompass();


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	testStepper();
	//testCompass();
}
