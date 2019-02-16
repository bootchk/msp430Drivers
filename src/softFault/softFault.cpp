
#include "softFault.h"

#include "../assert/myAssert.h"

/*
 * The assert writes line number to FRAM.
 * Multiple routines so we can debug by line number.
 *
 * In production, assert will SW reset SoC
 */
void SoftFault::failSetAlarm()   { Logger::log(1); myAssert(false);}

void SoftFault::failClearAlarm() { Logger::log(1); myAssert(false);}

void SoftFault::failReadTime()   { Logger::log(1); myAssert(false);}
