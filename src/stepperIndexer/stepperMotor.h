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
};
