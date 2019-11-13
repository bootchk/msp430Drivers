
// include types for time
#include "../time/rtcTime.h"  // RTCTime
#include "../time/timeTypes.h"  // EpochTime, Duration
#include "AB08xxRegisters.h"  // RTCAddress
//#include "../../bridge/bridge.h" // RegisterAddress

/*
 * Abstracts RTC methods.
 *
 * Implementation via read/write to registers of external device rtc via serial interface.
 * This hides:
 *  - registers and values
 *  - combinations of values required for certain actions
 *
 * Collaborates with Bridge, which hides SPI versus I2C
 */





class RTC {
public:
    /*
     * Configuration for alarming.
     * Standard: same as Linux driver: 24 hour mode, and alarm up to a year duration.
     */
    static bool configureStandardAlarming();

    /*
     * Basic sanity test.
     * Tests chain: MCU SPI to RTC chip, and RTC is alive.
     *
     * Can constant ID of RTC be read?
     */
    static bool isReadable();


	/*
	 * Clear flag that generated alarm interrupt.
	 * The flag must be cleared before another alarm can be set.
	 * Since RTC generates interrupt signal that is downward pulse (1/64 second),
	 * and interrupt on the mcu is rising edge,
	 * if this is called after the interrupt on MCU side,
	 * RTC should already have retuned high the Fout/nIRQ signal.
	 *
	 * Returns false on RTC bus errors.
	 *
	 * Caller should also check that Fout/nIRQ is high.
	 */
	static bool clearAlarmFlag();

	static bool isAlarmFlagClear();


	/*
	 * Return time from clock,
	 * or reset system if clock has failed.
	 */
	static EpochTime timeNowOrReset();


	/*
	 * Return true if alarm is set.
	 * Return false if:
	 * - duration too short
	 * - bus error (what was written did not compare to what was read)
	 *
	 * !!! Does not:
	 * - enable the alarm
	 * - ensure an alarm interrupt is configured
	 *
	 * The clock is still ticking.
	 * There can be a race to set a short alarm.
	 * To handle the race, this code requires duration >= 2 and assume the alarm can be set in less than one second.
	 * This code does not use Lamport's rule.
	 */
	// Set alarm duration from now
	static bool setAlarmDuration(Duration);

	// Set alarm to a time
	static bool setAlarmTime(EpochTime);


	/*
     * Verify alarm is properly set by reading and compare to given time (what was written earlier.)
     * If it is not set properly, the system may sleep a very long time.
     * Also verify that now time is not zero????
     *
     * The alarm register is not ticking, only the clock.
     */
	static bool verifyAlarmTime(const RTCTime*);


	// Set matching to no matching.
	static void disableAlarm();

	/*
	 * Are all aspects of alarm configured on RTC side?
	 */
	// Is configured to interrupt on alarm
	static bool isAlarmInterruptConfigured();

	// Is configured to set flag on alarm
	static bool isAlarmFlaggingConfigured();

	// Is alarm interrupt enabled on RTC chip
	static bool isAlarmInterruptEnabled();

	/*
	 * Configuration methods.
	 *
	 * At POR reset, the RTC should be:
	 * - running (CTRL1.stop is cleared)
	 * - in 12 hour mode
	 * - using XTAL oscillator mode
	 */


	static bool is24HourModeConfigured();

	/*
	 * Configure RTC to use an oscillator that is both precise and low power.
	 * See data sheet for details and other modes.
	 * If you don't call this, RTC defaults at reset to XTAL oscillator mode.
	 */
	static void configureRCCalibratedOscillatorMode();

	/*
	 * Configure mux in RTC so only alarm interrupts are on pin.
	 *
	 * This is optional:
	 * The reset condition of the RTC is that any interrupt will appear on the pin, if any interrupts are enabled?
	 * So this only restricts the interrupts to the alarm interrupt, and omits fault interrupts etc. ?
	 */
	static bool configureAlarmInterruptToFoutnIRQPin();

	static bool isOUTBitSet();



private:

	static void selectOscModeRCCalibratedWithAutocalibrationPeriod();
	static void enableAutocalibrationFilter();

	/*
	 * Configure alarm for match once per year
	 * AND enable alarm interrupt
	 */
	// OLD static void enablePulseInterruptForAlarm();
	// FUTURE duplicate with configureAlarmInterruptToFoutnIRQPin
	static bool connectFoutnIRQPinToAlarmSignal();

	static void unlockMiscRegisters();
	static void unlockOscControlRegister();


	/*
	* Used to check configuration.
	*/
	// Returns false if register cannot be read or can be read but does not have value
	static bool isRegisterHaveValue(RTCAddress, unsigned char value);
	// Returns false if register cannot be read or can be read but bits not set
	static bool isRegisterHaveBitsSet(RTCAddress, unsigned char maskValue);
	// Returns false if register cannot be read or can be read but bits not clear
	static bool isRegisterHaveBitsClear(RTCAddress, unsigned char maskValue);

	static bool isAlarmInterruptConfiguredPulse();
	static bool isAlarmConfiguredMatchPerYear();
	static bool isAlarmConfiguredToFoutnIRQPin();

};
