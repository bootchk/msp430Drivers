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
     * The chip is not immediately ready for SPI.
     * DRDY line goes high when ready.
     */
    static void turnPowerOn();
    static void turnPowerOff();

    /*
     * Returned a powered on chip to the reset state.
     */
    static void reset();

    static void configureForLowPowerSingleRead();

    /*
     * SPI and chip seems to be functioning.
     */
    static bool isSane();

    /*
     * If on and configured for continuous reading, returns most recent reading.
     * ??? are registers double buffered, or is this garbled?
     */
    static unsigned int readContinuousCompassHeading();

    /*
     * Configure for a single read of mag data,
     * then read, and convert to compass heading in degrees.
     */
    static unsigned int readSingleCompassHeading();

};
