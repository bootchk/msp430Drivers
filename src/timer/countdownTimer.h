
/*
 * Counts down from a given count and then interrupts.
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
