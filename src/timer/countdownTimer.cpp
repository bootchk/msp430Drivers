
#include "countdownTimer.h"

// Implementation uses Driverlib
#include <rtc.h>


/*
 * On MSP430 unsigned int is 16 bits max 65,535
 * Thus max duration yields pause of 650k uSec i.e. about .65 sec
 * 5000 ticks is .05 seconds
 * 500k cycles at 8mHz SMCLK is .05 seconds
 *
 */
void CountdownTimer::init(unsigned int durationInTicks)
{
    // Since clock is VLO at approx 10kHz, a tick is about 100uSec
    RTC_init(
        RTC_BASE,
        durationInTicks,  // compare reg value at which will trigger interrupt
        RTC_CLOCKPREDIVIDER_1);
}



void CountdownTimer::start() {
    RTC_clearInterrupt(RTC_BASE,
            RTC_OVERFLOW_INTERRUPT_FLAG);

    RTC_enableInterrupt(RTC_BASE,
            RTC_OVERFLOW_INTERRUPT);

    RTC_start(RTC_BASE, RTC_CLOCKSOURCE_VLOCLK);
}


void CountdownTimer::stop() {
    // This DriverLib implementation sets the clock source to 00 which I presume means none, and RTC disabled?
    RTC_stop(RTC_BASE);
}

