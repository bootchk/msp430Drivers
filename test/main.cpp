#include <msp430.h> 




void testStepper();
void testCompass();
void testCompass2();
void testRTC();
void testAlarm();
void testAlarm2();
void testTimer();
void testLEDSensor();
void testLEDSensor2();
void testStepperDriver();
void testLogging();




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//testStepper();
	//testCompass();
	//testRTC();
	//testCompass2();
	//testAlarm();
    //testAlarm2();
	testTimer();
	//testLEDSensor();
    //testLEDSensor2();
    //testStepperDriver();
	//testLogging();
}
