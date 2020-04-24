/*
 * High level driver for UV Light sensor
 *
 * Hides low-level driver.
 */

class UVSensor {
public:

    enum Error {
        I2CBusError,
        DeviceError,         // Wrong register or parameter address
        TimingError
    };


    /*
     * After power on, chip is in reset state.
     * The chip is ready for I2C after ??? sec

    static void turnPowerOn();
    static void turnPowerOff();


    static void configureForLowPowerSingleRead();
    */


    /*
     * There is no shutdown function, since we usually just power it off.
     */

    /*
     * I2C and chip seems to be functioning?
     *
     * Can be called whenever device is powered, without prior configuration.
     * Allow 5 mSec for device to power up.
     */
    static bool isSane();

    /*
     * Delays about 50 msec.
     */
    static unsigned int reset();



    /*
     * Configure for a single read of data
     * then read UVA and return value.
     *
     * Returns non-zero on error
     */
    static unsigned int readSingleUVA(unsigned int *UVResult);

};
