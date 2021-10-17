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

};
