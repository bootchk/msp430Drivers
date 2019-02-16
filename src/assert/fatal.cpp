
#include "fatal.h"

#include "../SoC/SoC.h"

#include "../logger/logger.h"

#include <cassert>

#include "../driverConfig.h"



#ifdef OLD
#include <msp430.h>

#include "../PMM/powerMgtModule.h"  // unlock LPM4.5

//#include "debug.h"

//#include "../peripheral/LEDAndLightSensor/ledAndLightSensor.h"


#endif


/*
 * myAssert writes line number to FRAM.
 * Multiple routines so we can debug by line number.
 *
 * In production, assert will SW reset SoC
 */



void Fatal::reboot(unsigned int reason) {
    // LED on briefly, you may fail to see
    //LEDAndLightSensor::toOnFromOff();
    //__delay_cycles(500000);

    Logger::log(reason);

    SoC::triggerSoftwareReset();
    /*
     * Continuation via reset vector.
     * That means C startup code, main() etc.
     * But reset reason will be SYSRSTIV_DOBOR.
     * main() must be equipped to handle reset properly, usually same as reason SYSRSTIV_BOR, power up.
     */
}


#ifdef FATAL_TESTING

void Fatal::fatalHWFault()                 { Fatal::warbleGreenLEDForever(); }
void Fatal::fatalSWFault()                 { Fatal::warbleGreenLEDForever(); }
void Fatal::fatalAssert(unsigned int line) { Fatal::warbleRedLEDForever(); }


#elif defined(FATAL_PRODUCTION)

void Fatal::fatalHWFault()                 { reboot(1); }
void Fatal::fatalSWFault()                 { reboot(2); }
void Fatal::fatalAssert(unsigned int line) { reboot(3); }

#elif defined(FATAL_DEBUGGING)

// No actual need to log, its here to test logging

// Delegate to C assert
void Fatal::fatalHWFault()                 { Logger::log(1); assert(false); }
void Fatal::fatalSWFault()                 { Logger::log(2); assert(false); }
void Fatal::fatalAssert(unsigned int line) {
    Logger::log(3);
    assert(false);
}

#endif


// TODO Debug::persistLineNumber(line);



