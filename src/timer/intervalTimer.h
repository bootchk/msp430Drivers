/*
 * Counts a duration and then interrupts.
 * Repeats until stopped, with same duration between interrupts.
 *
 * Duration is not a run-time parameter.
 *
 * Device WDT_A on MSP430
 *
 * !!! Requires
 */

class IntervalTimer {
public:
    // Only a limited set of intervals
    static void initForIntervalOf48Seconds();
    static void initForIntervalOfThreeSeconds();
    static void initForIntervalOfOneSecond();
    static void initForIntervalOfTenMillisecond();

    /*
     * Returns immediately.
     */
    static void start();

    /*
     * Does not guarantee that interrupt has not fired.
     * Ensures that if not already fired, will not fire.
     */
    static void stop();
};
