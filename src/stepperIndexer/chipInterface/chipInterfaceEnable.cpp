

/*
 * Part of class DriverChipInterface
 *
 * The part about sleep and enable outputs.
 */


#include "chipInterface.h"

#include "motor.h"

#include "../../delay/delay.h"




void
DriverChipInterface::delayForCommand() {
    Delay::oneMicrosecond();
}

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

    /*
     * assert after wake, chip step table is in "home" state, say 2
     */
    IndexerChipState::setDriverShadowStateToHomeStep();

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
}
#endif



/*
 * Pin is NotEnable: low is enabled, high is disabled.
 *
 * DriverChip has internal pulldown i.e. unconnected pin is in "enable" state
 */
void
DriverChipInterface::disableCoilDrive() {
    GPIO_setOutputHighOnPin(
            STEPPER_ENABLE_COILS_PORT,
            STEPPER_ENABLE_COILS_PIN);
    IndexerChipState::enableCoils(false);
    delayForCommand();
}
void
DriverChipInterface::enableCoilDrive() {
    GPIO_setOutputLowOnPin(
            STEPPER_ENABLE_COILS_PORT,
            STEPPER_ENABLE_COILS_PIN);
    IndexerChipState::enableCoils(true);
    delayForCommand();
}

bool
DriverChipInterface::isEnabledCoilDrive() {
    return IndexerChipState::isCoilsEnabled();
}


void
DriverChipInterface::configureIndexerMode() {
    GPIO_setOutputHighOnPin(
            STEPPER_CONFIG_MODE_PORT,
            STEPPER_CONFIG_MODE_PIN);
    /*
     * Short delay to settle.
     * Might not be necessary.
     * The chip will latch it on power up, or exit sleep.
     */
    delayForCommand();
}

void DriverChipInterface::unconfigureIndexerMode() {
    GPIO_setOutputLowOnPin(
            STEPPER_CONFIG_MODE_PORT,
            STEPPER_CONFIG_MODE_PIN);

}

