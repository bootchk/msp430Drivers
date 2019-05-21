
#include "counter.h"
#include "../assert/myAssert.h"


// DriverLib
#include <rtc.h>


namespace {

// Indicates overflow interrupt occurred.  Volatile: shared between ISR and this code.
volatile bool overflowFlag = false;

/*
 * Remember the parameter stored to modulo register.
 * Not accessible otherwise since:
 * - overflow resets the count register.
 * - DriverLib hides modulo register
 */
//unsigned int overflowCount;

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
    //overflowCount = durationInTicks;

    RTC_init(
        RTC_BASE,
        durationInTicks,  // compare reg value at which will trigger interrupt
        RTC_CLOCKPREDIVIDER_1);
    // assert predivider is set and clock is none and modulo register is set
    // not assert counter is zero
}



void Counter::enableAndClearInterrupt() {
    RTC_clearInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT_FLAG);

    RTC_enableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
}

void Counter::disableAndClearInterrupt() {
    RTC_disableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
    RTC_clearInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT_FLAG);
}


void Counter::start() {
    enableAndClearInterrupt();

    /*
     * This sets the clock source and resets the RTC.
     * Resetting clears the counter and reloads the modulo register.
     */
    RTC_start(RTC_BASE, RTC_CLOCKSOURCE_VLOCLK);

    overflowFlag = false;
}


void Counter::stop() {
    // Not require running

    // This DriverLib implementation of stop:
    // sets the clock source to 00 which I presume means: none and RTC disabled?
    // does not clear the counter (by setting RTCSR)
    RTC_stop(RTC_BASE);

    // assert IFG cleared by ISR if overflow occurred.

    RTC_disableInterrupt(RTC_BASE,
                RTC_OVERFLOW_INTERRUPT);

    // Not ensure counter is zero.
    // RTC IFG is clear since ISR clears it.
}


unsigned int Counter::getCount() {
    unsigned int result;



    if (overflowFlag) {
        // Require counter started with non-zero modulo
        // myAssert( overflowCount > 0);
        result = RTC_getModulo(RTC_BASE);
    }
    else {
        // modified MSPWare
        result = RTC_getCounter(RTC_BASE);
        // RTCCNT;
        /*
         * Even if the interrupt we are timing is quick,
         * the counter starts at 1 ???
         */
        myAssert( result > 0 );
    }
    return result;
}

// Called from ISR
void Counter::setOverflowFlag() { overflowFlag = true; }

