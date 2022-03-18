
/*
 * A high level API for a stepper behind a load switch,
 * that is unpowered between uses.
 *
 * When unpowered, a stepper cogs,
 * i.e. assumes a natural detent.
 *
 * This tries to keep the motor on a natural detent.
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
