
/*
 * Mangles RWBit (BIT7) of address of register on chip to indicate a read or write operation,
 * and multiple byte transfers (on some chips.)
 *
 * Modal with respect to RWBit.  Mode defaults to RWBit high for read.
 *
 * There is no formal SPI standard, the chips vary widely.
 * Different SPI chips use different schemes to indicate read/write.
 *
 * Some chips (LIS3MDL) use 5 bits of the byte for address.
 * Some chips (AB08x5) use a 6 bits of the byte for address.
 *
 * BIT7 means read or write.
 * BIT6 means multiple byte transfer.
 */

#include "bridge.h"  // BridgedAddress

enum class ReadOrWrite {
    Read,
    Write
};


class RegisterAddressMangler {
    // Whether the chip wants MSBit to be high on a read.
    static bool isRWBitHighForRead;

public:
    static unsigned char mangle(BridgedAddress, ReadOrWrite);

    static void configureRWBitHighForRead(bool);
};
