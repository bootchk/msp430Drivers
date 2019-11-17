ISR's Interrupt Service Routines to support msp430Driver modules.
The CCS IDE will make all other ISR's will call __TI_ISR_TRAP


Depends on:
- the msp430Drivers/config.h
- definitions for pins in board.h (which may be in an outer project)

The intent is to raise compile time errors if macro definitions don't exist needed by these modules,
and to make these ISR's trap if they are not used.

 If you omit needed ISR's (for which interrupts do occur) 
 the symptom is a call to isr_trap.ism, a TI supplied routine for missing ISR's.


Module            | ISR
-------------------------------
I2C                eUSCI_B
Timer              WDT or RTC0 (internal RTC)
LEDAndLightSensor  PORT1 or PORT2

This project does NOT use the NMI ISR.  You may define it somewhere else.



/*
 * ISR for interrupts on GPIO.
 *
 * !!! The GPIO pin for Alarm does interrupt.
 * The interrupt  wakes from LPM4.5,
 * But the ISR is never called because of the way we exit LPM4.5 (not enabling GPIO interrupt before clearing LPM5 bit.)
 *
 * This ISR is thus for servicing ISR for other GPIO pins.
 * Except for testing alarm, when this services an alarm from RTC (on pin 1.1?) and should wake from LPM3.
 */