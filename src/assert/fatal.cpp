#include "fatal.h"

#include "../SoC/SoC.h"

#ifdef OLD
#include <msp430.h>

#include "../PMM/powerMgtModule.h"  // unlock LPM4.5

//#include "debug.h"

//#include "../peripheral/LEDAndLightSensor/ledAndLightSensor.h"


#endif

// On production PCB
#define FATAL_PRODUCTION 1


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
