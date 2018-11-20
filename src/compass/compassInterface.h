
/*
 * Low level interface to compass chip.
 *
 * Here, only implement things I am interested in for low power, low performance.
 * Except chip is disabled for reading mag data (see set mode).
 * All registers should be readable after reset, but mag values bogus until set mode.
 *
 * At reset, most register contents default to a reasonable value.
 *
 *
 * Data ready bit and DRDY line.
 * Reading mag data (a certain byte) clears bit and DRDY line goes low.
 * Bit will be set and DRDY line goes high when the next data sample is ready.
 */

class CompassInterface {
public:
    /*
     * Test sanity of SPI and compass chip by reading chip ID.
     */
    static bool isReadable();

    /*
     * On chip reset, mode is "Power-down".
     * You must set mode after switching power to compass chip (HW reset) and before reading.
     */
    static void setSingleConversionMode();

    /*
     * Configure for 0.625Hz and one average per read.
     * Defaults to off (and then the DO bits control the ODR output data rate.)
     */
    static void setLowPowerMode();

    /*
     * Just a read.
     * Requires configured mode.
     *
     * If mode is singleConversion, a read returns chip to idle state.
     */
    static void readRawBytesOfMagneticData(unsigned char*);


    /*
     Is data ready bit in status register?
     This does NOT access the DRDY signal line.
     */
    static bool isDataReady();

    static unsigned char readStatus();

    /*
     * Likely other methods:
     * softwareReset();
     *
     */
};
