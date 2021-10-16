/*
 * Highest level API for stepper.
 *
 * It understands:
 * - wake/sleep
 * - acceleration and deceleration
 *
 */

#include "chipInterface/chipState.h"

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


    static bool isFault();

};
