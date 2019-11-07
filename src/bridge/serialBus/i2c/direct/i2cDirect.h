
/*
 * i2c without LPM or interrupts, i.e. polling for flags
 *
 * Derived from code on TI Forum by Clemens Ladisch
 */

class I2CDirect {
private:
    // wait for start and slave address sent
    // returns false if slave NACK (no slave recognizes slave address)
    static bool waitForStart();

public:

    /*
     * Read bytes from time register.
     */
    static void readTime();
    static unsigned int readID();

    /*
     * Read/write one byte to/from address in address space of slave.
     */
    static unsigned char readFromAddress(unsigned int registerAddress);
    static void writeToAddress(unsigned int registerAddress, const unsigned char value);

    static void readFromAddress(unsigned int registerAddress, unsigned char * const buffer, unsigned int count);
    static void writeToAddress(unsigned int registerAddress, const unsigned char * const buffer, unsigned int count);

    /*
     * Configure peripheral AND pins
     */
    static void init();

    /*
     * Ordinarily not called by public.
     */
    static void configurePins();
    static void configurePinsWithInternalPullups();

    /*
     * 7-bit right justified slave address
     */
    static void setSlaveAddress(unsigned int slaveAddress);

    static void initI2CPeripheral(unsigned int slaveAddress);
};
