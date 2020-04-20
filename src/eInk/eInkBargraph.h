
/*
 * Driver for eInk brand, 5 bar, bar gauge device, model number SCD722002
 *
 * Most operations may take seconds before isInkingComplete().
 *
 * If implementation is async, may require Timer ready for use.
 *
 * Requires persistent memory (FRAM) write is enabled.
 */
class EInkBargraph {
    /*
     * Delay long enough for electric field to change ink color.
     * Per eInk datasheet, 0.5 to 2 seconds.
     * This does not return immediately.
     */
    static void delayForInkSettling();




public:
    /*
     * Whenever we power up, the state of the ink is unknown.
     * (It may be a factory fresh device, or maybe in a different state if the device was used previously.)
     * Make the state of the ink known (off, all white), and make internal state match that.
     *
     * Only call this once, at POR power on reset.
     * For other kinds of reset
     * (like MSP430 reset for wake from cpu-off, RAM-off, LPM4.5)
     * this should NOT be called, and this driver persistently knows the state of the ink
     * (even though the device, the cpu, and memory were all unpowered.)
     *
     * !!! Requires FRAM is enabled for write.
     */
    static void configure();

    static void toggleSegment(unsigned int segment);

    // TODO
    static void turnOnSegment(unsigned int segment);
    static void turnOffSegment(unsigned int segment);

    // Has a prior async segment change completed?
    // TODO All operations are synchronous, with sleep.
    static bool isInkingComplete();

    static void allWhite();
    static void allBlack();

    /*
     * Clean up after an operation.
     * Unconfigure the GPIO pins.
     * After this, power usage goes to zero.
     * After this, isInkingComplete() will return true.
     *
     * This can be used in an ISR.
     */
    static void endInking();
};
