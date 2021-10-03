
/*
 * Knows how to handle slave devices that use register addresses.
 *
 * Hides whether the implementation uses combined transactions.
 *
 * Also hides the need to pass slaveAddress to deeper functions.
 *
 * This mostly creates new buffer so that it contains a prefix of the registerAddress
 *
 * Every method can return an error.
 *
 * Currently there are no convenience functions for single byte read/write
 */

/*
 Some devices use no register addresses for reads.
 For example, the Honeywell MPRLS pressure sensor.
 Instead, the protocol is to write to a register to start the device,
 and then to read from an unspecified register to get a combined status and measurement.
 */

class SlaveRegisterLayer {
public:
    // Read multiple bytes at a register address
    static bool read (unsigned int    slaveAddress,
                      unsigned int    registerAddress,
                      unsigned char * buffer,
                      unsigned int    count);

    // Ready multiple bytes without specifying a register address
    static bool readWORegister
                     (unsigned int    slaveAddress,
                      unsigned char * buffer,
                      unsigned int    count);

    // Write multiple bytes at a register address
    static bool write(unsigned int slaveAddress, unsigned int registerAddress, unsigned const char * const buffer, unsigned int count);
};

