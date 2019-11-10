
/*
 * API for C
 */

class DriverLibLinkWISR {
private:
    static void initForRead(unsigned char* buffer, unsigned int count);
    static void initForWrite(unsigned char* buffer, unsigned int count);

    // shared for read and write
    static void initI2CPins();
    static void setSlaveAddress();
    static void waitForPriorTransactionComplete();

public:
    static void readMultipleBytes(unsigned char * const buffer, unsigned int count);
    static void writeMultipleBytes(unsigned char * const buffer, unsigned int count);
};

