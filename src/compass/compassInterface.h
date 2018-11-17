
/*
 * Low level interface to compass chip.
 *
 * Here, only implement things I am interested in for low power, low performance.
 * At reset, most everything else defaults to a reasonable value.
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
     */
    static void readRawBytesOfMagneticData(unsigned char*);


    static bool isDataReady();

    /*
     * Likely other methods:
     * softwareReset();
     *
     */
};
