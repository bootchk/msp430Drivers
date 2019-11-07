
/*
 * Transport layer implemented using DriverLib Link layer.
 *
 * API is same as other transport layer implementations.
 *
 * Implementation does not use combined transactions.
 * A read from device takes two link operations: write register address to device, read from device.
 */


class I2CDriverLibLink {
private:
    // Link operations
    static void writeOneByte( unsigned char);
    static void writeMultipleBytes( unsigned int registerAddress, const unsigned char * const buffer, unsigned int count);

    static unsigned char readOneByte();
    static void readMultipleBytes( unsigned char * buffer, unsigned int count);

    static bool isStopComplete();

public:
    static void read(unsigned int registerAddress, unsigned char * buffer, unsigned int count);
    static void write(unsigned int registerAddress, unsigned const char * const buffer, unsigned int count);

    /*
     * Overloaded, single byte operations.
     */
    static unsigned char read(unsigned int registerAddress);
    static void write(unsigned int registerAddress, unsigned const char value);

    /*
     * Config
     */
    static void initI2CPeripheral(unsigned int slaveAddress);
};
