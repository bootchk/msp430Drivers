
/*
 * Drives a "sleepable" stepper motor using a DriverChip that implements "indexer" mode.
 *
 * Indexer mode: means main calls are setDirection() and stepDetent()
 * (instead of phase/enable)
 *
 * Sleepable: means that it can be slept in low power mode yet still single step from the detent motor position.
 * Includes ability to sleep in LMP4.5 (cpu totally unpowered) using MSP430 persistent FRAM.
 *
 *
 *
 * Missing features:
 *
 * No settable speed.
 * The application is single-stepping only (or at most a few steps).  Application does not care to accelerate motor speed faster than startup speed.
 * Speed is hard-coded at a low speed to match the max startup speed of the motor.
 * Each motor has a maximum startup speed specified (in PPS, pulse per second).
 * You should check that the hard-coded speed is less than the max your motor instance supports.
 * Generally, the lower the speed the more torque load it will start.
 *
 * Hides microstepping.  The API only allows detentSteps.
 * Microstepping is used in lower layers.
 * But you can't sleep at anything other than a detentStep, so API does not expose microsteps operation.
 *
 * No microstepping mode control.
 * Hard-coded to half-step mode.  I.E. is always two microsteps per detent step.
 * (The API is there to choose a different step mode, but the implementation assumes half-step mode.)
 *
 * Probably closed-loop feedback is still required.
 * The driver's notion of the motor position might not be reliable over a very long time.
 * (Motor might miss some steps.)
 *
 * Depends on DriverChipInterface, provided.
 * The provided DriverChipInterface only has a DriverLib implementation.
 *
 *
 *
 * Tested using MSP430 mcu and DRV8834 driver chip.
 * See
 *
 *
 * Algebra:
 *
 * At cold start, all pins should be set output low as soon as possible.
 * This sleeps the driver.
 *
 * At boot time, call initialWake(), syncDriverWithMotor()
 * Thereafter: sleep, wake, stepDetent, sleep, wake, setDirection, stepDetent, sleep, ....
 *
 * Also possible, to just spin the motor:
 * initialWake(), stepDetent(), stepDetent(), ...
 */






class StepperIndexer {
private:
    static void maintainShadowStep();

    static void delayAccordingToSpeed();

    /*
     * Shadowing
     */

    static void rememberMotorStep();

    /*
     * When you call this, you assert that motor step is in sync with given step.
     * Usually, you have wake() so that driver is on home step
     * and you have called microstep() through at least one cycle.
     */
    static void setShadowMicrostepOfDriver(unsigned int);

    /*
     * Advance shadow state from home step to current step without energizing coils.
     * This disables the driver outputs while stepping, to advance DriverChip internal state w/o moving motor.
     */
    static void restoreDriverToMotorStep();



    /*
     * Speed controlled microstep: built in delay.
     *
     * Does not enforce sleepable.
     * If you call it an odd count of times, then sleep, motor will twitch into a detent position.
     */
    static void stepMicrostep();

    /*
     * Step driver as fast as possible.
     * Driver should be disabled.
     * Motor does not move.
     */
    static void fastStepDetent();



public:

    /*
     * During this call, the motor will turn somewhat unpredictably.
     * Will sleep(reset) and wake
     *
     * Ensures driver is wake.
     */
    static void syncDriverWithMotor();

    static void initialWake();
    static void wake();
    static void sleep();


    /*
     * Step microsteps equivalent to a whole step, one that will detent if sleep (coils deenergized.)
     */
    static void stepDetent();

};
