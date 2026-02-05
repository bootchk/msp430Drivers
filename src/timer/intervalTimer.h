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
     *
     * VLO is 100uSec
     *
     * Divider   Interval
     * 64        6 mSec (~ten) implemented
     * 512       51 mSec
     * 8k        0.8 Sec (~1) implemented
     * 32k       3.2 Sec
     */
    static void initForIntervalOf48Seconds();
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
