
/*
 * Knows how to handle slave devices that use register addresses.
 *
 * Hides whether the implementation uses combined transactions.
 *
 * This mostly creates new buffer so that it contains a prefix of the registerAddress
 *
 * Every method can return an error.
 *
 * Currently there are no convenience functions for single byte read/write
 */


class SlaveRegisterLayer {
public:
    static bool read(unsigned int registerAddress, unsigned char * buffer, unsigned int count);
    static bool write(unsigned int registerAddress, unsigned const char * const buffer, unsigned int count);
};

