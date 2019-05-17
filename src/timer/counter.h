
/*
 * Counts up to a given count and then interrupts.
 * May interrupt again since counter keeps going and will wrap.
 * But the duration til the second interrupt is not the same duration as the first interrupt.
 *
 * Device "RTC" on older MSP430.   Which is a countdown timer, not a calendar.
 *
 * !!! Shares RTC with Timer (which does blocking delays.)
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
     * Between init() and start() should return 0?
     *
     * After start() and before overflow(), should return a value in range [0, durationInTicks]
     */
    static unsigned int getCount();

    /*
     * Stops ticking
     * AND disables interrupts.
     */
    static void stop();

    // called by ISR
    static void setOverflowFlag();
};
