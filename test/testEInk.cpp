
#include "../src/eInk/eInkBargraph.h"

#include "../src/SoC/SoC.h" // enable FRAM write

#include "../src/timer/timer.h"


// DriverLib
#include <pmm.h>

/*
static void delayHalfSecond() {
    __delay_cycles(500000); // half second
}
*/




#define USE_TIMER 1



static void delayTwoSeconds() {
#ifdef USE_TIMER
    LowPowerTimer::delaySeconds(2);
#else
    delayHalfSecond();
    delayHalfSecond();
    delayHalfSecond();
    delayHalfSecond();
#endif
}


static void allWhiteThenAllBlack() {
    delayTwoSeconds();

    //EInkBargraph::toggleSegment(1);
    EInkBargraph::allWhite();

    delayTwoSeconds();

    EInkBargraph::allBlack();
}




/*
 * Test EInk driver.
 *
 * In a loop,  toggle iInk segments
 * Expect: iInk segments to toggle
 *
 * Exception: if something hangs, will stop blinking.
 */
void testEInk()
{
    PMM_unlockLPM5();

    SoC::disableFRAMWriteProtect();

    allWhiteThenAllBlack();

    EInkBargraph::configure();

    while (true)
    {
        delayTwoSeconds();

        // allWhiteThenAllBlack();
        EInkBargraph::toggleSegment(1);

        delayTwoSeconds();

        EInkBargraph::toggleSegment(2);
    }
}
