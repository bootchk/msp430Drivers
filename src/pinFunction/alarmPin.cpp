
#include "alarmPin.h"

#include "../board.h"

#include "../assert/myAssert.h"

/*
 * Alarm pin
 */

void AlarmPin::configurePullupLoToHiInterrupt() {
    GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
    GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
}


void AlarmPin::enableInterrupt() {
    clearInterrupt();
    GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
}



/*
 * Hack for asserts only.
 * Not implemented
 * Should either use a variable in FRAM, or access port dir bit directly
 */
bool AlarmPin::isConfigured() {
    return true;
}

bool AlarmPin::isInterruptClear() {
    // hack not independent of port, fixed to port 1
    return not (P1IFG & AlarmSignalPin);
}

void AlarmPin::clearInterrupt() {
    GPIO_clearInterrupt(AlarmSignalPort, AlarmSignalPin);

    // If interrupt flag still set, the signaling device is still signaling
    myAssert(isInterruptClear());
}

bool AlarmPin::isHigh() {
    /*
     * GPIO_getInputPinValue returns a unsigned byte result for pin mask byte argument.
     * Non-zero result means AlarmSignalPin is high.
     */
    return (GPIO_getInputPinValue(AlarmSignalPort, AlarmSignalPin) != 0);
}
