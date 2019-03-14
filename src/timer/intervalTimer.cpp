#include "intervalTimer.h"

// DriverLib
#include "wdt_a.h"





void IntervalTimer::initForIntervalOfOneSecond() {
    /*
     * Using VLO at 10khz and divider of 8,192 yields 0.8 seconds
     */
    WDT_A_initIntervalTimer(WDT_A_BASE,
                            WDT_A_CLOCKSOURCE_VLOCLK,
                            WDT_A_CLOCKDIVIDER_8192);
}


void IntervalTimer::start() {
    WDT_A_start(WDT_A_BASE);

}
void IntervalTimer::stop() {
    WDT_A_hold(WDT_A_BASE);
}

