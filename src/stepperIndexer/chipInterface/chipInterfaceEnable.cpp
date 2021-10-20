

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



// TODO interrupt driven
void
DriverChipInterface::delayForWake() {
    Delay::oneMillisecond();
}



/*
 * Sleep functions optional at compile time.
 */

#if STEPPER_SLEEP_USED
/*
 * Toggle NotSleep pin.
 * Low is sleep.
 * High is wake (not sleep.)
 *
 * Low-level, not concerned with shadowing.
 */
void
DriverChipInterface::wake() {
    GPIO_setOutputHighOnPin(
            STEPPER_NSLEEP_PORT,
            STEPPER_NSLEEP_PIN);

    delayForWake();

    // assert after wake, chip step table is in "home" state, say 2
    // assert motor is at remembered motor step, not necessarily 2
    // assert driver chip is wake and reset
    // !!! but not enable coil drive

    // assert DriverChip in state 2
    // assert motor is at remembered motor step

    // TODO IndexerChipState::restoreDriverToMotorStep();
    // assert DriverChip is at same step as motor
    // assert coil drive enabled
}

void DriverChipInterface::sleep() {

    GPIO_setOutputLowOnPin(
            STEPPER_NSLEEP_PORT,
            STEPPER_NSLEEP_PIN);

    // TODO toSleep()
    IndexerChipState::rememberMotorStep();
}
#endif



/*
 * Pin is NotEnable: low is enabled, high is disabled.
 *
 * DriverChip has internal pulldown i.e. unconnected pin is in "enable" state
 */
void DriverChipInterface::disableCoilDrive() {
    GPIO_setOutputHighOnPin(
            STEPPER_ENABLE_COILS_PORT,
            STEPPER_ENABLE_COILS_PIN);
    IndexerChipState::enableCoils(false);
    delayForCommandChange();
}
void DriverChipInterface::enableCoilDrive() {
    GPIO_setOutputLowOnPin(
            STEPPER_ENABLE_COILS_PORT,
            STEPPER_ENABLE_COILS_PIN);
    IndexerChipState::enableCoils(true);
    delayForCommandChange();
}
