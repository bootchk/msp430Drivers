
#include "resetReason.h"

#include <msp430.h>

#include "../softFault/softFault.h"
//#include "../assert/myAssert.h"

//#include "../logger/logger.h"

/*
 * Called when the system is newly reset.
 * GPIO is locked. and GPIO to say LED's can't be used (unless you unlock and configure.)
 */


/*
 * SYSRSTIV is a generator: reading clears one reason but hw generates new reasons immediately
 * Many reasons can exist.
 *

MSP430FR2433

SYSRSTIV_NONE          (0x0000)        No Interrupt pending
SYSRSTIV_BOR           (0x0002)        SYSRSTIV : BOR
SYSRSTIV_RSTNMI        (0x0004)        SYSRSTIV : RST/NMI
SYSRSTIV_DOBOR         (0x0006)        SYSRSTIV : Do BOR
SYSRSTIV_LPM5WU        (0x0008)        SYSRSTIV : Port LPM5 Wake Up
SYSRSTIV_SECYV         (0x000A)        SYSRSTIV : Security violation
SYSRSTIV_RES12         (0x000C)        SYSRSTIV : Reserved
SYSRSTIV_SVSHIFG       (0x000E)        SYSRSTIV : SVSHIFG
SYSRSTIV_RES16         (0x0010)        SYSRSTIV : Reserved
SYSRSTIV_RES18         (0x0012)        SYSRSTIV : Reserved
SYSRSTIV_DOPOR         (0x0014)        SYSRSTIV : Do POR
SYSRSTIV_WDTTO         (0x0016)        SYSRSTIV : WDT Time out
SYSRSTIV_WDTPW         (0x0018)        SYSRSTIV : WDT Key violation
SYSRSTIV_WDTKEY        (0x0018)        SYSRSTIV : WDT Key violation
SYSRSTIV_FRCTLPW       (0x001A)        SYSRSTIV : FRAM Key violation
SYSRSTIV_UBDIFG        (0x001C)        SYSRSTIV : FRAM Uncorrectable bit Error
SYSRSTIV_PERF          (0x001E)        SYSRSTIV : peripheral/config area fetch
SYSRSTIV_PMMPW         (0x0020)        SYSRSTIV : PMM Password violation
SYSRSTIV_FLLUL         (0x0024)        SYSRSTIV : FLL unlock

MSP430FR6989

SYSRSTIV_FLLUL omitted

 */

bool ResetReason::isResetAWakeFromSleep() {
  bool done = false;
  bool result = false;

  while (! done)
  {
    // not using even_in_range() which is just an optimization, and dissappeared from compiler?
    unsigned int resetReason = SYSRSTIV;
    switch (resetReason)
    {
    case SYSRSTIV_NONE:
      done = true;  // stop loop - all reset reasons are cleared
      break;

    // Expected
    case SYSRSTIV_LPM5WU:
      result = true;
      break;

    // Expected
    case SYSRSTIV_BOR:     // power up from near zero volts (below 0.1V)
    case SYSRSTIV_RSTNMI:  // RST/NMI pin reset e.g. from debug probe
        break;

    // Unexpected, but comes from an assert() when assert() is configured to reset
    // We don't log it, since assert() should already have logged
    case SYSRSTIV_DOBOR:   // software initiated
      break;





    /*
     * All the rest are unexpected.
     */

    // Brownout
      // TODO since SVSH is off during sleep
      // will we get this during sleep?
      // we could also get this while awake, if we brownout and recover from it.
      // Maybe it should be treated as a normal cold reset.
    case SYSRSTIV_SVSHIFG:

    // Security. Accessing BSL that is protected. Probably wild memory access.
    case SYSRSTIV_SECYV:     // Security violation

     // WDT Time out
     // Not expected since we stop WD
     case SYSRSTIV_WDTTO:

    // Software initiated POR
    // Unexpected since our software never initiates.
    case SYSRSTIV_DOPOR:

    // Faults, abnormal e.g. "bus error"
    case SYSRSTIV_UBDIFG:    // FRAM Uncorrectable bit Error

    // Keys
    case SYSRSTIV_WDTKEY:    // WDT Key violation
    case SYSRSTIV_FRCTLPW:   // FRAM Key violation

    case SYSRSTIV_PERF:      // peripheral/config area fetch
    case SYSRSTIV_PMMPW:     // PMM Password violation
#ifdef __MSP430FR2433__
    case SYSRSTIV_FLLUL:     // FLL unlock
#endif

    default:
        // Log unexpected reset
        SoftFault::failHandleResetReason(resetReason);
        break;
    }
  }
  return result;
}
