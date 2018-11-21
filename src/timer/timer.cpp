
#include "timer.h"

// Implementation uses Driverlib
#include <cs.h>
#include <rtc.h>


/*
 * Use internal RTC instead of TIMER_A.
 * Rationale: lower power?
 *
 * !!! Note internal RTC vs external RTC
 */

namespace {

void initVLOClock()
{
    CS_initClockSignal(
         CS_FLLREF,
         CS_VLOCLK_SELECT  ,
         CS_CLOCK_DIVIDER_1);
}

/*
 * On MSP430 unsigned int is 16 bits max 65,535
 * Thus max duration yields pause of 650k uSec i.e. about .65 sec
 * 5000 ticks is .05 seconds
 * 500k cycles at 8mHz SMCLK is .05 seconds
 *
 */
void initRTC(unsigned int durationInRTCTicks)
{
    // Since clock is VLO at approx 10kHz, a tick is about 100uSec
    RTC_init(
        RTC_BASE,
        durationInRTCTicks,  // compare reg value at which will trigger interrupt
        RTC_CLOCKPREDIVIDER_1);
}



void startRTC() {
    RTC_clearInterrupt(RTC_BASE,
            RTC_OVERFLOW_INTERRUPT_FLAG);

    RTC_enableInterrupt(RTC_BASE,
            RTC_OVERFLOW_INTERRUPT);

    RTC_start(RTC_BASE, RTC_CLOCKSOURCE_VLOCLK);
}

void shutdownTimerResources() {
    // Let VLO stop when RTC stops using it
    CS_enableVLOAutoOff();

    // Stop RTC
    RTC_stop(RTC_BASE);
}



}


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



void LowPowerTimer::delayTicksOf100uSec(unsigned int ticks) {
    // Init the clock each time
    initVLOClock();

    // Init RTC each time
    initRTC(ticks);

    startRTC();

    // Enter low power until interrupt for RTC.
    // Does not return until RTC interrupt.
    // Since ISR exits low power, continues after this call.

    _low_power_mode_3();
    __no_operation();

    shutdownTimerResources();
    // Assert VLO clock is off and RTC counter is off
}


// 50k * 100uSec tick is 5 meg uSec is 5 seconds
void LowPowerTimer::delayFiveSeconds() { LowPowerTimer::delayTicksOf100uSec(50000); }

void LowPowerTimer::delayTwentyMilliSeconds() { LowPowerTimer::delayTicksOf100uSec(200); }

void LowPowerTimer::delayHalfSecond() { LowPowerTimer::delayTicksOf100uSec(5000); }

