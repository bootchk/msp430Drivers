
#include <msp430.h>

#include "../src/alarm/alarm.h"

// msp430Drivers
#include <board.h>  // board config




// Apparently this is C++ compatible

#ifdef NOT_USED
// Not used when warnings are errors
#if defined(__TI_COMPILER_VERSION__)
  // #warning "TI compiler"
  // unknown preprocessor directive: #message "TI compiler"
  // unrecognized pragma: #pragma message "foo"
#elif defined(__GNUC__)
  #warning "GNUC compiler"
#else
  #error Compiler not supported!
#endif
#endif


// For GNUC:  void __attribute__ ((interrupt(PORT2_VECTOR))) Port2_ISR (void)



/*
 * ISR for external RTC alarm interrupt.
 *
 * If alarm pin is Px, need ISR for Port x
 *
 * It is possible to eliminate this if you don't enable interrupt after sleep before unlocking.
 * Then the ISR is not called, even though interrupt occurred.
 *
 * Here, the ISR just clears interrupt flag, so no infinite interrupt loop.
 */


/*
 * This implementation is somewhat fragile, it only covers 3 GPIO ports
 *
 * !!! Also, it does not share the interrupt vector for other interrupts on the same port.
 */



#if  GPIO_PORT_P2 == AlarmSignalPort

#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    Alarm::clearAlarmOnMCU();
}
#elif GPIO_PORT_P1 == AlarmSignalPort

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    Alarm::clearAlarmOnMCU();

    // !!! for testing with LPM3 versus LPM4.5
    _low_power_mode_off_on_exit();
}

#elif GPIO_PORT_P3 == AlarmSignalPort

#ifndef PORT3_VECTOR
// !!! Some family members have no interrupts on port 3
#error PORT3_VECTOR not defined!
#endif

#pragma vector = PORT3_VECTOR
__interrupt void Port3_ISR(void)
{
    Duty::clearAlarmOnMCU();
}

#else

#error AlarmSignalPort not match any GPIO Port!

#endif






