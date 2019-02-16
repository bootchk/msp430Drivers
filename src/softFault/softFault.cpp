
#include "softFault.h"

#include "../assert/fatal.h"



void SoftFault::failSetAlarm()   { Fatal::fatalSWFault(); }

void SoftFault::failClearAlarm() { Fatal::fatalSWFault(); }

void SoftFault::failReadTime()   { Fatal::fatalSWFault(); }
