

/*
 * I2C where each transaction is single direction (not combined.)
 */

class I2CTransaction {
public:
    /*
     * Write a register address
     */
    static void writeByte(unsigned int slaveAddress, unsigned char data);

    /*
     * Read from whatever register was addressed (modal.)
     */
    static unsigned char readByte();





    /*
     * Combined transaction
     */
    static unsigned char receiveOneByte(unsigned int registerAddress);

    static void receiveMultipleBytes(unsigned int registerAddress,
                              unsigned char *buffer,
                              unsigned int length);
};
