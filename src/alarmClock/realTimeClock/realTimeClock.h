
// include types for time
#include "../time/rtcTime.h"  // RTCTime
#include "../time/timeTypes.h"  // EpochTime, Duration


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
     * Basic sanity test.
     * Tests chain: MCU SPI to RTC chip, and RTC is alive.
     *
     * Can constant ID of RTC be read?
     */
    static bool isReadable();


	/*
	 * Clear interrupt flag that generated alarm interrupt.
	 * The flag must be cleared before another alarm can be set.
	 * Since the interrupt is downward pulse (1/64 second)  from rtc,
	 * and interrupt on the mcu is rising edge,
	 * Fout/nIRQ should already be high.
	 *
	 * No error return.
	 * Caller must check that Fout/nIRQ is high.
	 */
	static void clearIRQInterrupt();


	/*
	 * Return time from clock,
	 * or reset system if clock has failed.
	 */
	static EpochTime timeNowOrReset();


	/*
	 * Return true if alarm is set.
	 * Return false if:
	 * - duration too short
	 * - SPI bus error (what was written did not compare to what was read)
	 *
	 * !!! Does not:
	 * - enable the alarm
	 * - ensure an alarm interrupt is configured
	 *
	 * The clock is still ticking.
	 * There can be a race to set a short alarm.
	 * This code has no special consideration for the race.
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

	/*
	 * Configure RTC so that match(counter, alarm) generates AIRQ signal.
	 * Match is on the entire alarm (i.e. once per year).
	 * Alternatively, the match can partial on the alarm, and generate AIRQ repeatedly e.g. once per second.
	 * AIRQ signal might be configured to generate an interrupt.
	 *
	 * For now, we configure it once at program start (cold reset.)
	 * We don't use this to disable alarm.
	 *
	 * The alarm only goes off once per setAlarm().
	 */
	static void configureAlarmMatchPerYear();

	/*
	 * Are all aspects of alarm configured on RTC side?
	 */
	// FUTURE
	static bool isAlarmConfigured();

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

	/*
	 * Configure hour counter to count to 24.
	 * !!! If you omit, upper nibble of hour counter is NOT hours tens digit, but include bit for AM/PM
	 */
	static void configure24HourMode();

	/*
	 * Configure RTC to use an oscillator that is both precise and low power.
	 * See data sheet for details and other modes.
	 * If you don't call this, RTC defaults at reset to XTAL oscillator mode.
	 */
	static void configureRCCalibratedOscillatorMode();

	/*
	 * Configure mux in RTC so only alarm interrupts are on pin.
	 */
	static void configureAlarmInterruptToFoutnIRQPin();

	static bool readOUTBit();

private:
	static void selectOscModeRCCalibratedWithAutocalibrationPeriod();
	static void enableAutocalibrationFilter();

	/*
	 * Configure alarm for match once per year
	 * AND enable alarm interrupt
	 */
	static void enablePulseInterruptForAlarm();
	static void connectFoutnIRQPinToAlarmSignal();

	static void unlockMiscRegisters();
	static void unlockOscControlRegister();


	/*
	* Used to check configuration.
	*/
	static bool isAlarmInterruptConfiguredPulse();
	static bool isAlarmConfiguredMatchPerYear();
	static bool isAlarmConfiguredToFoutnIRQPin();
	static bool isAlarmFlagClear();
};
