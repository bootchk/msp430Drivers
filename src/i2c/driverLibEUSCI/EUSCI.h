
/*
 * Code common to all I2C, link implementations.
 *
 * They all init the EUSCI peripheral the same way.
 * The differ in subsequent calls, to read and write.
 */
class EusciDriver {
public:
    static void initI2CPeripheral(unsigned int slaveAddress);
};
