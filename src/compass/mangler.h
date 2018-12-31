/*
 * Knows data conversions for compass
 */


class Mangler {
public:
    static unsigned int readCompassHeading();
    static void readAndDiscardToResetDataReady();

    //static void readRawBytesOfMagneticData();
    static void convertRawBytesToIntMagneticData(unsigned char rawMag[6], int mag[3]);
    static unsigned int convertThreeAxisIntMagneticDataToCompassHeading(int mag[3]);
};
