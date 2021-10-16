/*
 * Low-level interface to driver chip TI DRV8835
 */

#pragma once



#include "chipState.h"


class DriverChipInterface {
public:

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
     */
    static void wake();
    static void sleep();


    /*
     * Change direction of motor.
     *
     * Disables coils (so the field starts to decay.)
     * !!! Caller must enable coils before next step.
     *
     * Has a built-in delay.
     */
    static void setDirection(MotorDirection);
    static MotorDirection getDirection();

    /*
     * Disabled allows advancing current step in driver without moving motor.
     * Disabling saves power but gives smaller holding torque
     * and motor will assume a detent position (if using microsteps and not already at a detent step.)
     */
    static void disableCoilDrive();
    static void enableCoilDrive();


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

    static bool isFault();
    static void enableInterruptOnFault();
};
