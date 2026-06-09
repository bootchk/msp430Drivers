
#include <msp430.h>

#include "periodicInterrupt.h"


/*
Use the RTC module to periodically and continually interrupt.

The RTC module is in its own power domain
and is active in LPM3.5

This can conflict when RTC is used for LowPowerTimer or Counter.
*/

/*
Uses the VLO, which is not accurate
and can vary by 50% over voltage, temperature, and per chip variations.
Also, reduced by 15% for LPM3.

Here we don't calibrate, but do account for 15% drop,
since we are using the RTC to time sleep in LPM3.
*/

// Nominal 10k * 85%
#define VLONominalLPM3Freq 8500
// !!! This must match the symbolic value that sets the VLO divisor: RTCPS__1024
#define VLOClockDivisor 1024

/* Math from seconds to ticks. 

Use:
VLO at 10k hz nominal.
divisor on RTC module of 1024.
The clock runs in cycles, the RTC runs in ticks.

tick per sec == 
8.5 cycle per sec / 1024 cycle per tick = 8.3

sec per tick ==
1024 cycle per tick /8.5k cycles per sec = .12

512 ticks == 62 sec

Note the ticks need not be a power of 2.
*/
void 
PeriodicInterrupt::initInSeconds(uint16_t seconds)
{
    // Integer math. Cast to long 32-bit to avoid overflow.
    // Total clock cycles in period divided by clock divisor => ticks
    PeriodicInterrupt::initInTicks (((long) seconds * VLONominalLPM3Freq)
                                     / VLOClockDivisor);
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
