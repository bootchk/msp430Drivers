#include <msp430.h> 



void testStepper();
void testCompass();
void testCompass2();
void testRTC();
void testAlarm();
void testTimer();


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//testStepper();
	//testCompass();
	testRTC();
	//testCompass2();
	//testAlarm();
	//testTimer();
}
