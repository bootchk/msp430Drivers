/*
 * High level driver for STMicro LIS3MDL chip
 *
 * Hides low-level driver.
 * Knows how to convert raw compass readings to compass headings.
 */

class Compass {
public:
    /*
     * After power on, chip is in reset state.
     */
    static void turnPowerOn();
    static void turnPowerOff();

    /*
     * Returned a powered on chip to the reset state.
     */
    static void reset();

    static void configureForLowPowerSingleRead();

    static bool isReadable();

    /*
     * If on and configured for continuous reading, returns most recent reading.
     * ??? are registers double buffered, or is this garbled?
     */
    static unsigned int readContinuousCompassHeading();

    static unsigned int readSingleCompassHeading();

};
