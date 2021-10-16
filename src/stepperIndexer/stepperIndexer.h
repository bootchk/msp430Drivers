
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


#include "chipInterface/chipState.h"  // MotorDirection

enum class MotorSpeed {
    Max,
    Half,
    Quarter,
    Slow
};


class StepperIndexer {
private:

    static void delayFor100PPS();
    static void delayFor200PPS();
    static void delayFor250PPS();
    static void delayFor500PPS();
    static void delayFor6000PPS();

    /*
     * Delay for a run-time settable speed.
     */
    static void delayAccordingToSpeed();





    /*
     * Speed controlled microstep: built in delay.
     *
     * Does not enforce sleepable.
     * If you call it an odd count of times, then sleep, motor will twitch into a detent position.
     */
    static void stepMicrostepAtSpeed();

    /*
     * Step driver one detent step (may be many microsteps) as fast as possible.
     * Require coils disabled.
     * Motor does not move.
     */
    // TODO what is the purpose?
    static void fastStepDetent();



public:

    /*
     * ??? Not sure this is valid design???
     *
     * During this call, the motor will turn somewhat unpredictably.
     * Will sleep(reset) and wake
     *
     * Ensures driver is wake.
     */
    static void syncDriverWithMotor();

    /*
     * Rotate the motor enough to find a physical stop,
     * then reset the chip so at the home state.
     * Leaves:
     * - chip awake
     * - motor stationary at the hard stop.
     * - coils not energized
     * The arm must rest by gravity on the physical stop else since coils not energized, arm might move.
     */
    static void findPhysicalStop(MotorDirection);


    static void initialWake();
    static void wake();
    static void sleep();


    /*
     * Step microsteps equivalent to a whole step, one that will detent if sleep (coils deenergized.)
     *
     * !!! These are NOT speed controlled i.e. have no built-in delay.
     * If called faster than the driver chip supports, they may fail to drive motor properly.
     */
    static void stepDetentWithDelay(unsigned int delayMillseconds);

    /*
     * Same as above, but with a built-in delay that ensures frequency of steps is:
     * - less than the driver chip supports (e.g. 250 kHz or 4ms per step
     * !!! May be too fast to allow for acceleration.
     */
    static void stepDetentMaxSpeed();

    /*
     * Step a full step (to detent)
     * at a runtime settable speed less than the max startup speed (PPS) of the motor.
     * Might be called on the first step from stationary rotor condition.
     */
    static void stepDetentAtSpeed(MotorSpeed);

    static void stepManyDetentsAtSpeed(unsigned int detents, MotorSpeed);

    /*
     * Delay a microstep for given speed.
     */
    static void delayMicrostepForSpeed(MotorSpeed);

    /*
     * Delay a microstep for a compile time max motor speed.
     */
    static void delayMicrostepForMaxSpeed();

    /*
     * Delay enough that a max speed rotation stops and settles to a detent.
     */
    static void delayForSettling();


    static bool isFault();
};
