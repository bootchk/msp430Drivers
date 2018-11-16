/*
 * High level driver for STMicro LIS3MDL chip
 *
 * Hides low-level driver.
 * Knows how to convert raw compass readings to compass headings.
 */

class Compass {
public:
    static void turnPowerOn();
    static void turnPowerOff();

    static void reset();
    static bool isReadable();

    static unsigned int readCompassHeading();
};
