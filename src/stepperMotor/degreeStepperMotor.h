/*
 * API for a stepper motor, in degrees.
 *
 * This hides knowledge of step size in degrees.
 */

// implemented using stepper indexer drive chip
#include "../stepperIndexer/chipInterface/chipState.h"


// Does the API expose methods taking a count of steps?
#define STEP_API 0

class DegreeStepperMotor {
public:

    /*
     * General functions that delegate directly to StepperMotor
     *
     * See stepperMotor.h
     */
    static void wake();
    static void delayUntilDriverChipAwake();
    static bool isFault();



    static void findPhysicalStopAndHold(MotorDirection, MotorSpeed);


    /*
     * Turn the min amount that can be held, i.e. a step or microstep.
     * The min amount depends on the motor and the driver chip (whether it can microstep.)
     */
    static void turnAndHoldMinDegrees(MotorDirection direction, MotorSpeed speed);


    /*
     * Turn degrees in directions at speed.
     * Requires coils energized.
     * End with holding torque.
     */
    static void turnAndHoldDegrees(unsigned int degrees, MotorDirection direction, MotorSpeed speed);




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
};
