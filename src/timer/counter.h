
/*
 * Counts up to a given count and then interrupts.
 * May interrupt again since overflow resets counter but keeps ticking.
 *
 * Device "RTC" on older MSP430.   Which is a countdown timer, not a calendar.
 *
 * !!! Shares RTC with Timer (which does blocking delays.)
 *
 * You can use the RTC without interrupts, but this does use interrupts.
 */

class Counter {
public:
    static void init(unsigned int durationInTicks);

    /*
     * Returns immediately.
     * Enables interrupt.
     * Will continue to count and interrupt until stop() is called.
     */
    static void start();

    /*
     * Overflow resets counter (in HW, see datasheet.)
     * After overflow, this will return the durationInTicks (called "modulo value" in datasheet)
     * AKA the overflow value.
     *
     * Between init() and start() result is undefined.
     *
     * After start() and before overflow(), returns a value in range [0, durationInTicks].
     * May return zero:
     * - if you don't sleep between start() and getCount()
     * - OR the interrupt which wakes sleep comes before counter can tick (about 100 machine cycles at 1Mhz cpu clock and 10khz counter clock.)
     *
     * Any assertion that the count is greater than zero must be enforced by caller.
     */
    static unsigned int getCount();

    /*
     * Stops ticking
     * AND disables interrupts.
     */
    static void stop();

    // called by ISR
    static void setOverflowFlag();

    static void enableAndClearOverflowInterrupt();
    // called by ISR
    static void disableAndClearOverflowInterrupt();

};
