
#include "alarm.h"

#include "../realTimeClock/realTimeClock.h"

#include "../../pinFunction/alarmPin.h"    // hides GPIO functions

#include "../../assert/myAssert.h"
#include "../../softFault/softFault.h"






void Alarm::clearBothSidesOfSignal() {
        // May fail with a reset since RTC is remote device.
        Alarm::clearAlarmOnRTCOrReset();

        Alarm::clearAlarmOnMCU();
}




/*
 * Tell RTC to end interrupt pulse (signal to high) if not already so.
 * The RTC chip on alarm:
 * - asserts alarm pin high for a short time.
 * - sets a flag in the RTC
 * MCU is configured to interrupt on one edge.
 * When alarm pin transitions:
 * - MCU sets IFG
 * - calls ISR
 */
bool Alarm::clearAlarmOnRTC() {

	bool result;

	/*
	 * Simple write to a register of RTC.
	 * RTC should raise nIRQ signal to high if not already so.
	 *  mcu interrupt occurred on rising edge.
	 */
	if ( not RTC::clearAlarmFlag()) return false;

	// Signal is high
	result = isRTCReady();

    // assert alarm signal high or result is false
	return result;
}



void Alarm::clearAlarmOnRTCOrReset() {
	if (!Alarm::clearAlarmOnRTC())  {
		// RTC may be continuing to generate interrupt signal on Fout/nIRQ
		SoftFault::failClearAlarm();
	}
	// ensure RTC interrupt flag is clear
	// ensure interrupt signal net is high
}


void Alarm::clearAlarmOnMCU() {
	AlarmPin::clearInterrupt();
}

bool Alarm::isClearOnMCUSide() {
    return AlarmPin::isInterruptClear();
}


bool Alarm::isRTCReady() {
	// requires pin configured as input
	return AlarmPin::isHigh();
}

bool Alarm::isRTCReadable() {
    return RTC::isReadable();
}





/*
 * Must be bulletproof since if alarm is failed to set, may sleep forever.
 */
bool Alarm::setAlarmDurationSecondsFromNow(Duration duration) {

    /*
     * Does not require (duration >=0).  It will bump duration to 2 if less than 2.
     * Does not require duration < DriverConstant::MaxPracticalAlarmDuration), will return false if so.
     */
	bool result;

	myAssert(isConfiguredForAlarming());

	// delegate to RTC
	result = RTC::setAlarmDuration(duration);

	// We enable interrupt even if alarm was not set, but caller should handle.
	// Clear interrupt flag and enable at last moment
	AlarmPin::enableInterrupt();

	// ensure alarm is set or result is false
	return result;
}



#ifdef OLD
bool Alarm::setAlarmToTime(EpochTime time) {
    bool result;

    myAssert(isConfiguredForAlarming());

    // delegate to RTC
    result = RTC::setAlarmTime(time);

    // Clear interrupt flag and enable at last moment
    AlarmPin::enableInterrupt();

    // ensure alarm is set or result is false
    return result;
}
#endif

