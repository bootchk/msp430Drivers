
/*
 * Mangles address of register on chip to indicate a read or write operation,
 * and multiple byte transfers (on some chips.)
 *
 *
 * Modal.  Mode defaults.
 *
 * There is no formal SPI standard, the chips vary widely.
 * Different SPI chips use different schemes to indicate read/write.
 *
 * Some chips (LIS3MDL) use a 5-bit address and BIT6 means multiple byte.
 * BIT7 means read or write.
 */

#include "bridge.h"  // BridgedAddress

enum class ReadOrWrite {
    Read,
    Write
};


class RegisterAddressMangler {
    // Whether the chip wants MSBit to be high on a read.
    static bool isReadHigh;

public:
    static unsigned char mangle(BridgedAddress, ReadOrWrite);
};
