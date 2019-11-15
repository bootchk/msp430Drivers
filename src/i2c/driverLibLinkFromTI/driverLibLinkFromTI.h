
/*
 * Implementation of I2C derived from TI DriverLib example
 *
 * Uses interrupts, ISR vector for eUSCI_B
 *
 * Most routines return an error.
 * Since errors can happen (although extremely remote if sw is correct):
 * - NACK
 * - loss of bus arbitration (other master)
 *
 * The only read/write calls are for multiple bytes.
 * Of course you can just use count of 1.
 */

class DriverLibLinkWISR {
private:
    static void initForRead(
            unsigned int slaveAddress,
            unsigned char* buffer,
            unsigned int count);
    static void initForWrite(
            unsigned int slaveAddress,
            unsigned char* buffer,
            unsigned int count);

    // shared for read and write
    static void initI2CPins();
    static void setSlaveAddress(unsigned int slaveAddress);

    // FUTURE this should have a timeout and return an error
    static void waitForPriorTransactionComplete();

public:
    // returns true on success
    static bool readMultipleBytes(
            unsigned int slaveAddress,
            unsigned char * const buffer,
            unsigned int count);
    static bool writeMultipleBytes(
            unsigned int slaveAddress,
            unsigned char * const buffer,
            unsigned int count);
};

