#include "fatal.h"

#include <msp430.h>

#include "../PMM/powerMgtModule.h"  // unlock LPM4.5

//#include "debug.h"

//#include "../peripheral/LEDAndLightSensor/ledAndLightSensor.h"
#include "../SoC/SoC.h"


// On production PCB
#define FATAL_PRODUCTION 1

/*
 * Testing on launchpad.
 * Enter a tight loop with green or red led blinking so we can debug.
 */
//#define FATAL_TESTING 1





/*
 * Terminal routines: never return.
 * To indicate faults.
 * In testing, asserts and NMI ISR's call these.
 * In production, asserts and NMI ISR's
 */

void Fatal::warbleRedLEDForever() {
    ensureRedLEDLightable();

    // warble
    while(true) {
        P1OUT ^= BIT0;                      // toggle P1.0
        __delay_cycles(50000);
    }
}


void Fatal::warbleGreenLEDForever() {
    ensureGreenLEDLightable();

    // warble
    while(true) {
        P1OUT ^= BIT1;                      // toggle P1.1
        __delay_cycles(50000);
    }
}


void Fatal::abortGreenLED() {
    ensureGreenLEDLightable();

    P1OUT != BIT1;      // P1.1 is high, LED is sourced
    ///TestMain::lightGreenLED();
    // spin
    while(true) ;
}


void Fatal::ensureGreenLEDLightable() {
    // Ensure GPIO config will have effect
    PMM::unlockLPM5();

    // GPIO configured out
    P1DIR |= BIT1;
}

void Fatal::ensureRedLEDLightable() {
    // Ensure GPIO config will have effect
    PMM::unlockLPM5();

    // GPIO configured out
    P1DIR |= BIT0;
}


void Fatal::reboot() {
    // LED on briefly, you may fail to see
    //LEDAndLightSensor::toOnFromOff();
    //__delay_cycles(500000);

    SoC::triggerSoftwareReset();
    /*
     * Continuation via reset vector.
     * That means C startup code, main() etc.
     * But reset reason will be SYSRSTIV_DOBOR.
     * main() must be equipped to handle reset properly, usually same as reason SYSRSTIV_BOR, power up.
     */
}



void Fatal::fatalReset() {
#ifdef FATAL_TESTING
    Fatal::warbleGreenLEDForever();
#elif defined(FATAL_PRODUCTION)
    reboot();
#endif
}

void Fatal::fatalAssert(unsigned int line) {
    // TODO Debug::persistLineNumber(line);

#ifdef FATAL_TESTING
    Fatal::warbleRedLEDForever();
#elif defined(FATAL_PRODUCTION)
    reboot();
#endif

}
