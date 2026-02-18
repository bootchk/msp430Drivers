#include "intervalTimer.h"

// DriverLib
#include "wdt_a.h"
#include <sfr.h>



/*
Interval Timer uses WDT peripheral
Requires WDT not used for watchdog reset purpose.

Uses VLO clock which is 10kHz
Tick is 100uSec
     
     Divider   Interval
     64        6 mSec (~ten) implemented
     512       51 mSec
     8k        0.8 Sec (~1) implemented
     32k       3.2 Sec
     512k      48 Sec
*/

static void
initIntervalTimerDivider (unsigned int divider)
{
    WDT_A_initIntervalTimer(WDT_A_BASE,
                            WDT_A_CLOCKSOURCE_VLOCLK,
                            divider);
}


void IntervalTimer::initForIntervalOf48Seconds() {
    // divider of 512k yields 48 seconds
    initIntervalTimerDivider(WDT_A_CLOCKDIVIDER_512K);
}

void IntervalTimer::initForIntervalOfThreeSeconds() {
    // divider of 32k yields 3 seconds
    initIntervalTimerDivider(WDT_A_CLOCKDIVIDER_32K);
}
void IntervalTimer::initForIntervalOfOneSecond() {
    // divider of 8,192 yields 0.8 seconds
    initIntervalTimerDivider(WDT_A_CLOCKDIVIDER_8192);
}

void IntervalTimer::initForIntervalOfTenMillisecond() {
    // divider of 64 yields 6.4 milli seconds
    initIntervalTimerDivider(WDT_A_CLOCKDIVIDER_64);
}


void IntervalTimer::start() {
    // Make counter start counting
    WDT_A_start(WDT_A_BASE);

    // Enable interrupt from counter
    SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
}
void IntervalTimer::stop() {
    WDT_A_hold(WDT_A_BASE);
    SFR_disableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
}

