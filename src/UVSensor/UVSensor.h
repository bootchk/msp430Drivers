/*
 * High level driver for VEML6975 Light sensor
 *
 * Hides low-level driver.
 */

class UVSensor {
public:
    /*
     * After power on, chip is in reset state.
     * The chip is ready for I2C after ??? sec
     */
    static void turnPowerOn();
    static void turnPowerOff();


    static void configureForLowPowerSingleRead();

    /*
     * I2C and chip seems to be functioning.
     */
    static bool isSane();



    /*
     * Configure for a single read of data
     * then read UVA and return value.
     */
    static unsigned int readSingleUVA();

};
