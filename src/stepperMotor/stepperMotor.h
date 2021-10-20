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
     * Turn at maximum speed after one half speed step.
     * End with holding torque.
     */
    static void turnAndHold(unsigned int steps, MotorDirection direction);

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
     * then reset the chip so at the home state.
     * Leaves:
     * - chip awake
     * - motor stationary at the hard stop.
     * - coils not energized
     * The arm must rest by gravity on the physical stop else since coils not energized, arm might move.
     */
    static void findPhysicalStop(MotorDirection);

};
