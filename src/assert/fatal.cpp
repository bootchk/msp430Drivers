
#include "fatal.h"

#include "../SoC/SoC.h"

#include <cassert>

#include "../driverConfig.h"



#ifdef OLD
#include <msp430.h>

#include "../PMM/powerMgtModule.h"  // unlock LPM4.5

//#include "debug.h"

//#include "../peripheral/LEDAndLightSensor/ledAndLightSensor.h"


#endif






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


#ifdef FATAL_TESTING

void Fatal::fatalReset()                   { Fatal::warbleGreenLEDForever(); }
void Fatal::fatalAssert(unsigned int line) { Fatal::warbleRedLEDForever(); }


#elif defined(FATAL_PRODUCTION)

void Fatal::fatalReset()                   { reboot(); }
void Fatal::fatalAssert(unsigned int line) { reboot(); }

#elif defined(FATAL_DEBUGGING)

// Delegate to C assert
void Fatal::fatalReset()                   { assert(false); }
void Fatal::fatalAssert(unsigned int line) { assert(false); }

#endif


// TODO Debug::persistLineNumber(line);



