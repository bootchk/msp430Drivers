#include <msp430.h> 



/*
void testStepper();
void testStepperIndexer();
void testPicker();

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
void testLED();
void testLEDSensor2();
void testLEDSensor3();

void testLogging();
void testI2C();
void testI2CTransport();
void testUVSensor();
void testUVSensor2();
void testEInk();
void testPressureSensor();
void testStepperDriver();
void testLoadSwitch();
void testPressureSensor();

void testPicker();
void testStepperIndexer();

void testSunDial();
*/

void testSunDial2();



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//testStepper();
	//testStepperIndexer();
	//testPicker();
	//testSunDial();
	testSunDial2();


	//testCompass();
	//testCompass2();

	//testTimer();

	//testLED();
	//testLEDSensor();
    //testLEDSensor2();
	//testLEDSensor3();

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
	//testUVSensor2();

	//testEInk();

	//testPressureSensor();
	//testLoadSwitch();
}
