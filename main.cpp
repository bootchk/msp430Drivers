#include <msp430.h> 



void testStepper();
void testCompass();
void testRTC();


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//testStepper();
	testCompass();
	// testRTC();
}
