
/*
 * Low level interface to compass chip.
 */

class CompassInterface {
public:
    static void readRawBytesOfMagneticData(unsigned char*);
    static bool isReadable();
};
