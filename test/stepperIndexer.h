
/*
 * Drives a stepper motor using a DriverChip that implements "indexer" mode.
 *
 * DriverChip
 */




// depends on motor
//#define STEPS_PER_REV 200
#define MOTOR_STEPS_PER_REV 20




/*
 * Value is microsteps per detent step.
 */
enum class StepMode {
    Full = 1,
    Half = 2,
    Quarter = 4
};







class StepperIndexer {
public:

    static unsigned int stepsPerRev();


    /*
     * Commands (high/low state) to DriverChip
     */

    // Stepping mode
    static void toFullStepMode();
    static void toHalfStepMode();
    static void toQuarterStepMode();

    // Sleep is a reset, especially the current step, to "home state"
    static void wake();
    static void sleep();


    static void toggleDirection();


    /*
     * Disabled allows advancing current step in driver without moving motor.
     */
    static void disableOutput();
    static void enableOutput();



    /*
     * Speed
     */
    static void delayAccordingToSpeed();


    /*
     * Shadow step
     */



    /*
     * Tell driver to step one microstep.
     *
     * !!! This will not actually step motor if driver is disabled.
     * But it always advances the shadow step.
     */
    // TODO rename stepMicrostep
    static void microstep();

    /*
     * Step microsteps equivalent to a whole step, one that will detent if coils deenergized.
     */
    static void stepDetent();


    /*
     * Shadowing
     */

    static void rememberMotorStep();

    /*
     * When you call this, you assert that motor step is in sync with given step.
     * Usually, you have wake() so that driver is on home step
     * and you have called microstep() through at least one cycle.
     */
    static void setShadowStepOfDriver(unsigned int);

    /*
     * Advance shadow state from home step to current step without energizing coils.
     */
    static void restoreDriverToMotorStep();
};
