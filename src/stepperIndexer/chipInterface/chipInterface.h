/*
 * Low-level interface to driver chip TI DRV8835
 */



/*
 * Size of a microstep.
 * Value is microsteps per detent step.
 *
 * A detent is where the motor stops if not driven
 * Where the residual magnetism stops the rotation.
 *
 * Some driver chips and boards allow step size to be set at runtime.
 * Whether the board does  (whether hardwired to a specific step size)
 * is defined in board.h, conditional compilation.
 */
enum class StepSizeMode {
    Full = 1,
    Half = 2,
    Quarter = 4
};


enum class MotorDirection {
    Forward,
    Backward
};


class DriverChipInterface {
public:

    static unsigned int microstepsPerRev();
    static unsigned int detentstepsPerRev();


    // Stepsize mode
    static void toFullStepSizeMode();
    static void toHalfStepSizeMode();
    static void toQuarterStepSizeMode();

    static StepSizeMode getStepSize();

    /*
     * Sleep saves power.
     *
     * Sleep is a reset, especially the current microstep, to "home state"
     *
     * Datasheet is not clear that sleep also disables outputs.
     *
     * Built-in delay for wake.
     */
    static void wake();
    static void sleep();


    /*
     * Change direction of motor.
     * Has a built-in delay.
     */
    static void setDirection(MotorDirection);
    static MotorDirection getDirection();

    /*
     * Disabled allows advancing current step in driver without moving motor.
     * Disabling saves power but gives smaller holding torque
     * and motor will assume a detent position (if using microsteps and not already at a detent step.)
     */
    static void disableCoilDrive();
    static void enableCoilDrive();


    /*
     * Tell chip to step one microstep.
     * Pulse high the "step" pin.
     *
     * A microstep might not be a full detent step.
     *
     * !!! This will not actually step motor if driver output is disabled.
     * This does not understand shadow step.
     *
     * This does not have any delays to control speed.
     */
    static void stepMicrostep();
    // inline TODO doesn't link when O3
};
