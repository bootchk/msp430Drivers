#include <msp430.h> 




void testStepper();
void testCompass();
void testCompass2();
void testRTC();
void testAlarm();
void testAlarm2();
void testAlarmLPM45();
void testTimer();
void testLEDSensor();
void testLEDSensor2();
void testStepperDriver();
void testLogging();
void testI2C();
void testI2CTransport();




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//testStepper();
	//testCompass();
	//testCompass2();

	//testTimer();
	//testLEDSensor();
    //testLEDSensor2();
    //testStepperDriver();
	//testLogging();

	// These tests are usually run in this order, from low level implementation to high

	//testI2CTransport();
	//testI2C();
	//testRTC();
	testAlarm();
	//testAlarm2();
	//testAlarmLPM45();
}
