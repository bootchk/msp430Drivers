
#include "fatal.h"

#include "../SoC/SoC.h"

#include "../logger/logger.h"

#include <cassert>
#include <msp430.h>

#include "../config/driverConfig.h"



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


/*
 * Parameter is typically the line of an assert that failed.
 * But could be codes [1,2] for other faults, see below.
 *
 */
void Fatal::reboot(unsigned int line) {
    // LED on briefly, you may fail to see
    //LEDAndLightSensor::toOnFromOff();
    //__delay_cycles(500000);

    Logger::log(line);

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
void Fatal::fatalAssert(unsigned int line) { reboot(line); }

#elif defined(FATAL_DEBUGGING)

// No actual need to log, its here to test logging


/*
 * We could call assert, but the definition provided by TI attempts to write() to the host and calls abort().
 * We don't want to drag in write().
 */
void Fatal::stop(FailCode logCode) {


    /*
     * If we are in the debugger, logging is not necessary.
     * But if we are not now in debugger, log so we can connect later and read the code.
     */
    Logger::log(static_cast<unsigned int> (logCode));

    /*
     * Break into the debugger (aka BKPT) if it is running, else NOP.
     * The EEM on MSP430 looks for this opcode.
     */
    __op_code(0x4343);

    /*
     * Infinite loop.
     * If not in the debugger, lack of blinking should alert you to fact is stopped.
     * Should also exhaust power and Vcc to alert you something is wrong.
     */
    while (true) ;
}


void Fatal::fatalHWFault()                 { stop(FailCode::HWFault); }
void Fatal::fatalSWFault(FailCode code)                 { stop(code); }
void Fatal::fatalAssert(unsigned int line) {
    Logger::log(line);
    stop(FailCode::Assert);
}

#endif


// TODO Debug::persistLineNumber(line);



