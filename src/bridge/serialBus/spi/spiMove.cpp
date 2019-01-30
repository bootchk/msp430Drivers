
/*
 * data moving methods of spi
 */

#include "spi.h"





namespace {

/*
 * Transfer many bytes over the bus.
 *
 * An SPI.transfer is duplex or bidirectional: both read and write in the same transfer.
 * On write, we ignore what is read from slave.
 *
 * Note that Serial does not implement any multi-byte transfers.
 * Instead, here we iterate over single byte transfers.
 */
void innerReadBuffer( unsigned char * bufferPtr, unsigned int size) {
    // require slave already selected
    for(unsigned int i = 0; i < size; i++) {
        /*
         * Transfer as a read requires dummy byte here a distinctive pattern.
         * Expect return value is from slave
         */
        bufferPtr[i] = SPI::transferDuplex( 0b10101010 );    // nonsense value
    }
}

void innerWriteBuffer( unsigned char * bufferPtr, unsigned int size) {
    // require slave already selected
    for(unsigned int i = 0; i < size; i++) {
        // ignore returned read
        (void) SPI::transferDuplex(bufferPtr[i]);
    }
}

} // namespace







/*
 * Moves are in terms of duplex transfers.
 * Certain transfers we ignore what is passed to write or what is read.
 *
 * This implements iteration for multiple bytes, in units of one byte transfers.
 */

void SPI::read(const RegisterAddress registerAddress,
               unsigned char * const bufferIn,
               const unsigned int count) {

    // require isConfigured()
    // require slave selected

    // mangle register address to indicate read of register
    RegisterAddress mangled;
    mangled = Serial::mangleRegisterAddress(ReadOrWrite::Read, registerAddress);

    // write registerAddress
    // ignore returned result
    (void) SPI::transferDuplex(mangled);

    // transfer data bytes
    innerReadBuffer(bufferIn, count);
}



#ifdef TODO
void Bridge::writeBuffer(RegisterAddress registerAddress,
                        unsigned int length,
                        unsigned char * source) {

    myRequire(isConfigured());

    Serial::selectSlave(chosenDevice);
    RegisterAddress mangled;
    mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);
    // write registerAddress
    (void) Serial::transfer(ReadOrWrite::Write, mangled);
    // transfer the data bytes
    innerWriteBuffer(source, length);
    Serial::deselectSlave();
}
#endif
