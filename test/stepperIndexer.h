
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
     * Step
     */
    static void step();
};
