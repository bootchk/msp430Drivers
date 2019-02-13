/*
 * Low-level interface to driver chip TI DRV8835
 */



/*
 * Value is microsteps per detent step.
 */
enum class StepMode {
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


    // Stepping mode
    static void toFullStepMode();
    static void toHalfStepMode();
    static void toQuarterStepMode();

    // Sleep is a reset, especially the current step, to "home state"
    static void wake();
    static void sleep();


    static void setDirection(MotorDirection);
    static MotorDirection getDirection();

    /*
     * Disabled allows advancing current step in driver without moving motor.
     */
    static void disableOutput();
    static void enableOutput();


    /*
     * Tell driver to step one microstep.
     *
     * !!! This will not actually step motor if driver output is disabled.
     * This does not understand shadow step.
     */
    static void stepMicrostep();
};
