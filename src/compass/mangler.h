/*
 * Knows data conversions for compass
 */


class Mangler {
public:
    static unsigned int readCompassHeading();

private:
    static void readRawBytesOfMagneticData();
    static void convertRawBytesToIntMagneticData();
    static unsigned int convertThreeAxisIntMagneticDataToCompassHeading();
};
