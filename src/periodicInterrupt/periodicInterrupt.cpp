
#include <msp430.h>

#include "periodicInterrupt.h"


/*
Use the RTC module to periodically and continually interrupt.

The RTC module is in its own power domain
and is active in LPM3.5

This can conflict when RTC is used for LowPowerTimer or Counter.
*/



/* Math from seconds to ticks. 

Use:
VLO at 12k hz
divisor on RTC module of 1024 

tick per sec == 
12k cycle per sec / 1024 cycle per tick = 11.7 

sec per tick ==
1024 cycle per tick /12k cycles per sec = .0853 

512 ticks = 44.7 sec
32 ticks = 2.7 sec

Note the ticks need not be a power of 2.
*/
void 
PeriodicInterrupt::initInSeconds(uint16_t seconds)
{
    // Integer math. Cast to long 32-bit to avoid overflow.
    PeriodicInterrupt::initInTicks (((long) seconds * 12000) / 1024);
}


void 
PeriodicInterrupt::initInTicks(uint16_t ticks)
{
    // Configure RTC
    // VLO clock source @ 12kHz

    // Interrupt and reset happen every 1024/12000 * ticks sec.
    RTCMOD = ticks-1;
    RTCCTL = RTCSS__VLOCLK | RTCSR |RTCPS__1024;

    // Enable interrupt from device, but not the GIE
    RTCCTL |= RTCIE;
}
