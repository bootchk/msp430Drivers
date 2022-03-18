/*
 * Low-level interface to driver chip TI DRV8835
 */

#pragma once



#include "chipState.h"


class DriverChipInterface {
public:


    /*
     * Delay for any command (other than wake) till next step
     *
     * Commands are GPIO changes to the chip:
     * - direction change
     * - enable outputs
     * - microstep mode (usually hardwired, not runtime)
     *
     * Chip spec says 1 uS
     */
    static void delayForCommand();

    /*
     * Chip spec requires delay after any wake before chip is active.
     * Ensure 1 milliSec before any subsequent operations.
     */
    static void delayForWake();

    // getters
    static unsigned int microstepsPerRev();
    static unsigned int detentstepsPerRev();


    // Stepsize mode
    static void toFullStepSizeMode();
    static void toHalfStepSizeMode();
    static void toQuarterStepSizeMode();

    static StepSizeMode getStepSize();

    /*
     * Sleep saves power.
     *
     * Sleep is a reset, especially the current microstep, to "home state"
     *
     * Datasheet is not clear that sleep also disables outputs.
     *
     * Built-in delay for wake.
     *
     * Only sets/clears a GPIO.  Does NOT maintain shadow state.
     */
    // Defined, but optionally conditionally compiled out
    static void wake();
    static void sleep();


    static MotorDirection getDirection();

    /*
     * Change direction of motor.
     *
     * Disables coils (so the field starts to decay.)
     * !!! Caller must enable coils before next step.
     *
     * Has a built-in delay.
     */
    static void setDirectionAndRelease(MotorDirection);

    /*
     * Change direction of motor.
     *
     * Coils remain enabled (holding torque.)
     *
     * Has NO built-in delay.
     */
    static void setDirectionAndHold(MotorDirection);



    /*
     * Disabled allows advancing current step in driver without moving motor.
     * Disabling saves power but gives smaller holding torque
     * and motor will assume a detent position (if using microsteps and not already at a detent step.)
     */
    static void disableCoilDrive();
    static void enableCoilDrive();
    static bool isEnabledCoilDrive();


    /*
     * Tell chip to step one microstep.
     * Pulse high the "step" pin.
     *
     * A microstep might not be a full detent step.
     *
     * !!! This will not actually step motor if driver output is disabled.
     * This does not understand shadow step.
     *
     * This does not have any delays to control speed.
     */
    static void stepMicrostep();
    // inline TODO doesn't link when O3


    // Defined, but optionally conditionally compiled out
    static bool isFault();
    static void enableInterruptOnFault();

    /*
     * Set GPIO that holds DRV8834 pin Config high (chip in indexer mode)
     */
    static void configureIndexerMode();
    /*
     * Set corresponding GPIO low, so won't draw current in LPM.
     * Usually called before unpowering DRV8834 chip.
     */
    static void unconfigureIndexerMode();

};
