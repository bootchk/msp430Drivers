

#include "alarmPin.h"

#include "../assert/myAssert.h"

// App
#include <board.h>

/*
 * Alarm pin
 */

void AlarmPin::configurePullupLoToHiInterrupt() {
    GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
    /*
     * RTC floats signal (pullup high) until alarm, and drains it low for a short duration.
     */
    GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
    // GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_HIGH_TO_LOW_TRANSITION);
}


void AlarmPin::enableInterrupt() {
    clearInterrupt();
    GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
}

// FUTURE hardcoded
bool AlarmPin::isEnabledInterrupt() {
    // P1.1
    return (P1IE & BIT1);
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
    /*
     * Implementation is independent of choice of alarm port.
     * But certain ports on certain family members (P3 on MSP430FR2433) do not generated interrupts!!!!
     */
    uint16_t interruptBits;
    interruptBits = GPIO_getInterruptStatus(AlarmSignalPort, AlarmSignalPin);
    return interruptBits == 0;

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
