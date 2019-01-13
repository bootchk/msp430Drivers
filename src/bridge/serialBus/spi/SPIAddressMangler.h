
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
 *
 * This is only for SPI.  I2C uses a RW bit outside the register address.
 * ??? I2C uses different bit mangling of register address to indicate multiple byte reads?
 */

#include "../serial.h" // ReadOrWrite
#include "../../bridge.h"  // RegisterAddress


class SPIRegisterAddressMangler {
    // Whether the chip wants MSBit to be high on a read.
    static bool isRWBitHighForRead;

public:
    static unsigned char mangle(RegisterAddress registerAddress, ReadOrWrite);

    static void configureRWBitHighForRead(bool);
};
