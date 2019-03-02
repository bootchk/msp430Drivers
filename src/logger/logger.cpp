
#include "logger.h"

#include "../assert/myAssert.h"

namespace {

/*
 * Both the data and index are persistent.
 * Never overwritten except by program load.
 */
#pragma PERSISTENT
unsigned int logData[4] = {0,0,0,0};

#pragma PERSISTENT
unsigned int logIndex = 0;
}



void Logger::log(unsigned int value) {
    if ( logIndex < 4) {
        logData[logIndex] = value;
        logIndex++;
    }
}

void Logger::logError(unsigned int value) {
    log(value);
    myAssert(false);
}
