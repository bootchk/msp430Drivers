

/*
 * Part of class DriverChipInterface
 *
 * The part about sleep and enable outputs.
 */


#include "chipInterface.h"

#include "motor.h"

#include "../../delay/delay.h"



namespace {

/*
 * Chip spec requires delay after any wake before chip is active.
 * Ensure 1 milliSec before any subsequent step.
 *
 * TODO interrupt driven
 */
void delayForWakeChange() {
    Delay::oneMillisecond();
}

/*
 * Delay for any command (other than wake) till next step
 *
 * ??? Not sure what commands are
 * Commands are:
 * - direction change
 * - enable outputs
 * - microstep mode (usually hardwired, not runtime)
 *
 */
void delayForCommandChange() {
    Delay::oneMicrosecond();
}

} // namespace





/*
 * Toggle NotSleep pin.
 *
 * Low-level, not concerned with shadowing.
 */
void DriverChipInterface::wake() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4);

    delayForWakeChange();

    // assert DriverChip in state 2
    // assert motor is at remembered motor step, not necessarily 2
}

void DriverChipInterface::sleep() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
}




/*
 * Pin is NotEnable: low is enabled, high is disabled.
 *
 * DriverChip has internal pulldown i.e. unconnected pin is in "enable" state
 */
void DriverChipInterface::disableCoilDrive() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}
void DriverChipInterface::enableCoilDrive() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}
