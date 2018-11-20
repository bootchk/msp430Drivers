
#include <src/softFault/softFault.h>

#include "../assert/myAssert.h"

/*
 * The assert writes line number to FRAM.
 * Multiple routines so we can debug by line number.
 *
 * In production, assert will SW reset SoC
 */
void SoftFault::failSetAlarm() {myAssert(false);}

void SoftFault::failClearAlarm() {myAssert(false);}

void SoftFault::failReadTime() {myAssert(false);}
