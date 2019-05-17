
#include "counter.h"


// DriverLib
#include <rtc.h>


namespace {

// Indicates overflow interrupt occurred.  Shared between ISR and this code.
bool overflowFlag = false;

/*
 * Remember the parameter stored to modulo register.
 * Not accessible otherwise since:
 * - overflow resets the count register.
 * - DriverLib hides modulo register
 */
unsigned int overflowCount;

}



/*
 * Uses VLO at approx 10kHz: a tick is about 100uSec
 * On MSP430 unsigned int is 16 bits max 65,535
 * Thus max duration yields duration of 6.50M uSec i.e. about 6.5 sec
 * 5k ticks is 500kuSec is .5 seconds
 *
 * RTC is periodic.
 * It will interrupt repeatedly unless stopped.
 * Overflow clears counter register in HW, and reloads modulo value (compare register.)
 */
void Counter::init(unsigned int durationInTicks)
{
    overflowCount = durationInTicks;

    RTC_init(
        RTC_BASE,
        durationInTicks,  // compare reg value at which will trigger interrupt
        RTC_CLOCKPREDIVIDER_1);
}



void Counter::start() {
    RTC_clearInterrupt(RTC_BASE,
            RTC_OVERFLOW_INTERRUPT_FLAG);

    RTC_enableInterrupt(RTC_BASE,
            RTC_OVERFLOW_INTERRUPT);

    RTC_start(RTC_BASE, RTC_CLOCKSOURCE_VLOCLK);

    overflowFlag = false;
}


void Counter::stop() {
    // This DriverLib implementation sets the clock source to 00 which I presume means none, and RTC disabled?
    RTC_stop(RTC_BASE);

    // assert IFG already cleared

    RTC_disableInterrupt(RTC_BASE,
                RTC_OVERFLOW_INTERRUPT);
}


unsigned int Counter::getCount() {
    if (overflowFlag)  return overflowCount;
    else return RTCCNT;
}

// Called from ISR
void Counter::setOverflowFlag() { overflowFlag = true; }

