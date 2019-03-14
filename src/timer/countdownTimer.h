
/*
 * Counts up to a given count and then interrupts.
 * May interrupt again since counter keeps going and will wrap.
 * But the duration til the second interrupt is not the same duration as the first interrupt.
 *
 * Device "RTC" on older MSP430.
 *
 * Which is a countdown timer, not a calendar.
 */

class CountdownTimer {
public:
    static void init(unsigned int durationInTicks);
    static void start();
    static void stop();
};
