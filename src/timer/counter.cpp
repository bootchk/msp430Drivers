
#include "counter.h"
#include "../assert/myAssert.h"


// DriverLib
#include <rtc.h>


namespace  {

// Indicates overflow interrupt occurred.  Volatile: shared between ISR and this code.
volatile bool overflowFlag = false;

/*
 * Remember the parameter stored to modulo register.
 *
 * DriverLib hides modulo register but I modified DriverLib to make it accessible.
 * But
 */
unsigned int overflowCount;

}



/*
 * Uses VLO at approx 12kHz: a tick is about 100uSec
 * On MSP430 unsigned int is 16 bits max 65,535
 * Thus max duration yields duration of 6.50M uSec i.e. about 6.5 sec
 * 5k ticks is 500kuSec is .5 seconds
 *
 * RTC is periodic.
 * It will interrupt repeatedly unless stopped.
 * Overflow clears counter register in HW, and reloads modulo value (compare register.)
 *
 * RTC has pre-dividers of 1,10, .. and 1,64,256,...
 */

void Counter::initWithDivisor(unsigned int durationInTicks,
                              unsigned int divisor)
{
    /*
     * Can be arbitrarily short, but not zero (which makes no sense.)
     * If too short and a fast clock, ISR and other code may delay actual service for event,
     * IOW this is not a good solution for timing short events.
     */
    myRequire(durationInTicks>0);
    // Since unsigned int, durationInTicks < 65k

    // Redundant to modulo register
    overflowCount = durationInTicks;

    RTC_init(
        RTC_BASE,
        durationInTicks,  // compare reg value at which will trigger interrupt
        divisor);
    // assert predivider is set and clock is none and modulo register is set
    // not assert counter is zero
}

void Counter::init12kHz(unsigned int durationInTicks)
{
    initWithDivisor(durationInTicks, RTC_CLOCKPREDIVIDER_1);
}

void Counter::init1_2kHz(unsigned int durationInTicks)
{
    initWithDivisor(durationInTicks, RTC_CLOCKPREDIVIDER_10);
}



void Counter::enableAndClearOverflowInterrupt() {
    RTC_clearInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT_FLAG);

    RTC_enableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
}

void Counter::disableAndClearOverflowInterrupt() {
    RTC_disableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
    RTC_clearInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT_FLAG);
}


void Counter::start() {
    enableAndClearOverflowInterrupt();

    /*
     * Set the clock source and reset the RTC.
     * Resetting clears the counter and reloads the shadow reg from modulo register.
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

    disableAndClearOverflowInterrupt();

    // Not ensure counter is zero.
    // RTC IFG is clear
}


unsigned int Counter::getCount() {
    unsigned int result;

    if (overflowFlag) {
        /*
         * Overflow occurred.
         * RTC reset counter to zero and reloaded shadow register.
         */
        // Require counter started with non-zero modulo
        result = RTC_getModulo(RTC_BASE);
    }
    else {
        // modified MSPWare
        result = RTC_getCounter(RTC_BASE);    // RTCCNT
        /*
         * If the interrupt we are timing is quick,
         * the counter may still be zero
         */
    }

    // Result is always positive, but may be zero.
    myAssert( result <= overflowCount );

    return result;
}

// Called from ISR
void Counter::setOverflowFlag() { overflowFlag = true; }

