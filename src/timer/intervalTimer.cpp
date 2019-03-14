#include "intervalTimer.h"

// DriverLib
#include "wdt_a.h"
#include <sfr.h>





void IntervalTimer::initForIntervalOfOneSecond() {
    /*
     * Using VLO at 10khz and divider of 8,192 yields 0.8 seconds
     */
    WDT_A_initIntervalTimer(WDT_A_BASE,
                            WDT_A_CLOCKSOURCE_VLOCLK,
                            WDT_A_CLOCKDIVIDER_8192);
}


void IntervalTimer::initForIntervalOfTenMillisecond() {
    /*
         * Using VLO at 10khz and divider of 64 yields 6.4 milli seconds
         */
        WDT_A_initIntervalTimer(WDT_A_BASE,
                                WDT_A_CLOCKSOURCE_VLOCLK,
                                WDT_A_CLOCKDIVIDER_64);
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

