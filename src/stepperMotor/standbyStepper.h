
/*
 * A high level API for a stepper behind a load switch,
 * that is unpowered between uses.
 */

class StandbyStepperMotor {
public:

    /*
     * Step, then unpower.
     * Assert the position is only held by residual magnetism or bearing friction
     * but momentum is zero so we won't overshoot.
     */
    static void powerOnAndStepThenOff();

    /*
     * Power on, with built-in delay for chip ready.
     * Datasheet does not specify a timing requirement.
     */
    static void powerOn();


};
