
/*
 * Drives a stepper motor using a DriverChip that implements "indexer" mode.
 *
 * DriverChip
 */






class StepperIndexer {
public:




    static void wake();
    static void sleep();



    /*
     * Speed
     */
    static void delayAccordingToSpeed();


    /*
     * Shadow step
     */





    /*
     * Step microsteps equivalent to a whole step, one that will detent if sleep (coils deenergized.)
     */
    static void stepDetent();

    /*
     * Does not enforce sleepable.
     * If you call it an odd count of times, then sleep, motor will twitch into a detent position.
     */
    static void stepMicrostep();


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
