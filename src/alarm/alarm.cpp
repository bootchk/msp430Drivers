
#include "alarm.h"

#include "../realTimeClock/realTimeClock.h"

#include "../pinFunction/alarmPin.h"    // hides GPIO functions

#include "../assert/myAssert.h"
#include "../softFault/softFault.h"






void Alarm::clearBothSidesOfSignal() {
        // May fail with a reset since RTC is remote device.
        Alarm::clearAlarmOnRTCOrReset();

        Alarm::clearAlarmOnMCU();
}





bool Alarm::clearAlarmOnRTC() {
	// Tell RTC to end interrupt pulse (signal to high) if not already so
	bool result;

	/*
	 * Simple write to a register of RTC.
	 * RTC should raise nIRQ signal to high if not already so.
	 *  mcu interrupt occurred on rising edge.
	 */
	RTC::clearIRQInterrupt();

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





/*
 * Must be bulletproof since if alarm is failed to set, may sleep forever.
 */
bool Alarm::setAlarmDurationSecondsFromNow(Duration duration) {
	bool result;

	myAssert(isConfiguredForAlarming());

	// delegate to RTC
	result = RTC::setAlarmDuration(duration);

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

