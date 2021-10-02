
#include "timer.h"

#include "../clock/veryLowOscillator.h"

#include <msp430.h> // LPM3 macro

#include "../config/driverConfig.h"    // choose implementation

#ifdef LOW_POWER_TIMER_USE_RTC
#include "../timer/counter.h"
#else
#include "intervalTimer.h"
#endif

#include "../assert/myAssert.h"

/*
 * Use internal RTC instead of TIMER_A.
 * Rationale: lower power?
 *
 * !!! Note internal RTC vs external RTC
 */




/*
 * LPM3 ram retention, CPU stopped, all clocks off (except VLO), core regulator on, RTC interrupt
 *
 * LPM3.5 no ram retention, core regulator off, external OR RTC interrupt
 *
 * LPM4 is ram retention, CPU stopped, all clocks off, core regulator on, external interrupt only
 *
 * LPM4.5 no ram retention, core regulator off, external interrupt only
 */
/*
 * Here, we don't mess with 3.5 since we want continuation here (instead of a BOR.)
 */


#ifdef LOW_POWER_TIMER_USE_RTC

void LowPowerTimer::delayTicksOf100uSec(unsigned int ticks) {
    // Init the clock each time
    // Nov. 15, 2019 This is suspect: see implementation, it does not really start the VLO, the VLO turns on by itsefl?
    // VeryLowOscillator::start();

    // Init RTC each time
    Counter::init12kHz(ticks);

    Counter::start();
    // Race to sleep before counter interrupts

    // Enter low power until interrupt for RTC.
    // Does not return until RTC interrupt.
    // Since ISR exits low power, continues after this call.

    // Nov. 14, 2019 Trying mode 0 does not affect crash of setAlarm()
    //_low_power_mode_0();

    // Can tolerate extra wakeup time from LPM3
    _low_power_mode_3();

    __no_operation();

    // shutdown resources(
    // Let VLO stop when RTC stops using it
    // FUTURE not neccessary
    VeryLowOscillator::allowOff();
    Counter::stop();

    // Assert VLO clock is off and RTC counter is off
}


// 50k * 100uSec tick is 5 meg uSec is 5 seconds
void LowPowerTimer::delayFiveSeconds()        { LowPowerTimer::delayTicksOf100uSec(50000); }
void LowPowerTimer::delayHalfSecond()         { LowPowerTimer::delayTicksOf100uSec(5000); }

void LowPowerTimer::delayTwentyMilliSeconds() { LowPowerTimer::delayTicksOf100uSec(200); }
void LowPowerTimer::delayTenMilliSeconds()    { LowPowerTimer::delayTicksOf100uSec(100); }
void LowPowerTimer::delayTwoMilliSeconds()    { LowPowerTimer::delayTicksOf100uSec(20); }

void LowPowerTimer::delaySeconds(unsigned int count) {
    /*
     * Param to delayTicks is unsigned int, max is 64k.
     * So seconds must be less than 7.
     */
    myRequire(count < 7);
    LowPowerTimer::delayTicksOf100uSec(count*10000);
}

#endif


#ifdef LOW_POWER_TIMER_USE_WDT

void LowPowerTimer::delaySecond() {
    // Init the clock each time
    VeryLowOscillator::start();
    // Init RTC each time
    IntervalTimer::initForIntervalOfOneSecond();

    IntervalTimer::start();

    // Enter low power until interrupt for RTC.
    // Does not return until RTC interrupt.
    // Since ISR exits low power, continues after this call.

    _low_power_mode_3();
    __no_operation();

    // shutdown resources(
    // Let VLO stop when RTC stops using it
    VeryLowOscillator::allowOff();
    IntervalTimer::stop();

}


void LowPowerTimer::delayTenMilliSeconds() {
   // See comments above
    VeryLowOscillator::start();
    IntervalTimer::initForIntervalOfTenMillisecond();
    IntervalTimer::start();
    _low_power_mode_3();
    __no_operation();
    VeryLowOscillator::allowOff();
    IntervalTimer::stop();
}

/*
 * Implentation is iteration, with setup/shutdown and interrupt in each iteration.
 */
void LowPowerTimer::delaySeconds(unsigned int count) { for (unsigned int i = count; i >0; i--) delaySecond(); }

void LowPowerTimer::delayFiveSeconds() { for (unsigned int i = 5; i >0; i--) delaySecond(); }



#endif
