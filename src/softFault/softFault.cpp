
#include "softFault.h"

#include "../assert/fatal.h"
#include "../logger/logger.h"


/*
 * These create one log word
 */
void SoftFault::failSetAlarm()   { Fatal::fatalSWFault(FailCode::SWFaultSetAlarm); }

void SoftFault::failClearAlarm() { Fatal::fatalSWFault(FailCode::SWFaultClearAlarm); }

void SoftFault::failReadTime()   { Fatal::fatalSWFault(FailCode::SWFaultReadTime); }

void SoftFault::failDetectLight()   { Fatal::fatalSWFault(FailCode::SWFaultDetectLight); }


/*
 * These create two log words.
 */
void SoftFault::failHandleResetReason(unsigned int resetCode)   {
    // resetCode is defined by MSP430 HW
    Logger::log(resetCode);
    Fatal::fatalSWFault(FailCode::SWUnhandledReset);
}

void SoftFault::info(unsigned int infoCode)   {
    // infoCode is defined by application
    Logger::log(infoCode);
    // Not fatal, but log the code that means previous log is info
    Logger::log(static_cast<unsigned int> (FailCode::SWInfo) );
}


