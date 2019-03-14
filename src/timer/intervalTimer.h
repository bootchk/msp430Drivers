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
    /*
     * Only a limited set of intervals
     */
    static void initForIntervalOfOneSecond();

    static void start();
    static void stop();
};
