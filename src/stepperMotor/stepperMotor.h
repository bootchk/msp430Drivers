/*
 * Highest level API for stepper.
 *
 * It understands:
 * - wake/sleep
 * - acceleration and deceleration
 *
 */

// implemented using stepper indexer drive chip
#include "../stepperIndexer/chipInterface/chipState.h"

class StepperMotor {
public:

    /*
     * Wake driver chip,
     * when driver chip can sleep under MCU control.
     */
    static void wake();

    /*
     * Delay a time that ensures the driver chip is awake.
     *
     * Can be used at power up to ensure driver chip is awake,
     * when hardwired awake and wake() is not called.
     */

    static void delayUntilDriverChipAwake();

    /*
     * Turns at maximum rate, ending in coils deenergized and sleep.
     *
     * Includes all delays for accelleration, speed, and decelleration.
     *
     * When done, motor rotor is stopped at a detent and motor deenergized.
     */
    static void wakeTurnAndSleep(unsigned int steps, MotorDirection direction);

    /*
     * Turn full steps at maximum speed.
     * If steps >, first step is at half speed.
     * End with holding torque.
     */
    static void turnAndHold(unsigned int steps, MotorDirection direction);

    /*
     * Turn micro steps at maximum speed.
     * End with holding torque.
     */
    static void turnAndHoldMicrosteps(unsigned int steps, MotorDirection direction);

    /*
     * Turn quarter rev with accel and decel.
     * End condition is sleep i.e. no holding torque.
     */
    static void wakeTurnAcceleratedQuarterRevAndSleep(MotorDirection direction);

    /*
     * Turn quarter rev with accel and decel.
     * End condition is holding torque.
     */
    static void turnAcceleratedQuarterRevAndHold(MotorDirection direction);

    /*
     * Turn the given steps, direction, then hold.
     */
    static void turnAcceleratedStepsAndHold(unsigned int stepCount, MotorDirection direction);


    /*
     * Poll the fault bit.  Not useful on some driver chips because the fault resets itself.
     */
    static bool isFault();

    /*
     * Rotate the motor enough to find a physical stop,
     * Hold torque.
     *
     * When the physical stop is not aligned with a detent step,
     * or if there are external forces,
     * if you subsequently release torque the motor rotor may move away from the stop.
     */
    static void findPhysicalStopAndHold(MotorDirection);

    /*
     * Reset the chip so at the home state.
     * Leaves:
     * - chip awake
     * - coils not energized
     * - SW model of the step state at "home"
     *
     * - motor stationary at the hard stop.
     *
     * When:
     * - the arm rests by gravity on the physical stop
     * - the physical stop is at a detent step
     * Then the SW model of the step state (at "home")
     * will be accurate to the actual state of the motor rotor.
     *
     * When those conditions are not true (re the physical stop)
     * since coils not energized, the motor rotor might move,
     * and then the SW model of the step state is NOT accurate
     * to the actual state of the motor rotor.
     */
    static void syncMotorWithModel();


    /*
     * Reverse direction, step half a detent step (typically 9 degrees),
     * reverse direction, step half a detent step (typically 9 degrees).
     * Ends in same position as started, in same direction as started.
     * Usually the starting position was a detent step.
     *
     * All with coils enabled (holding torque.)
     */
    static void jiggle();

};
