/*
 * Knows data conversions for compass
 */


class Mangler {
public:
    static unsigned int readCompassHeading();
    static void readAndDiscardToResetDataReady();

private:
    static void readRawBytesOfMagneticData();
    static void convertRawBytesToIntMagneticData();
    static unsigned int convertThreeAxisIntMagneticDataToCompassHeading();
};
