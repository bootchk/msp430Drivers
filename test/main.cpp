#include <msp430.h> 




void testStepper();
void testCompass();
void testCompass2();
void testRTC();
void testRTC2();
void testRTCAlarmPoll();
void testRTCAlarmPoll2();
void testRTCTimeMonotonic();
void testAlarm();
void testAlarm2();
void testAlarm3();
void testAlarmLPM45();
void testAlarmLPM45AndTimer();
void testTimer();
void testLEDSensor();
void testLEDSensor2();
void testStepperDriver();
void testLogging();
void testI2C();
void testI2CTransport();
void testUVSensor();
void testUVSensor2();
void testEInk();




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
	//testRTC2();
	//testRTCAlarmPoll();
	//testRTCAlarmPoll2();
	//testRTCTimeMonotonic();
	//testAlarm();
	//testAlarm2();
	//testAlarm3();
	//testAlarmLPM45();
	//testAlarmLPM45AndTimer();
	//testUVSensor();
	testUVSensor2();
	//testEInk();
}
