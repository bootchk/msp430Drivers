
/*
 * i2c without LPM, i.e. polling for flags
 *
 * Not general.  A hack.  Used for testing.
 *
 * Derived from code on TI Forum by Clemens Ladisch
 */

class I2CDirect {
public:

    /*
     * Read bytes from time register.
     */
    static void readTime();
    static unsigned int readID();

    /*
     * Read from address in address space of slave.
     */
    static unsigned int readFromAddress(unsigned int registerAddress);

    /*
     * Configure peripheral AND pins
     */
    static void init();

    /*
     * Ordinarily not called by public.
     */
    static void configurePins();

    /*
     * 7-bit right justified slave address
     */
    static void setSlaveAddress(unsigned int slaveAddress);
};
