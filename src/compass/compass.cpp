
#include "compass.h"

void Compass::turnPowerOn() { }

void Compass::turnPowerOff() { }

void Compass::reset() { }

int Compass::readCompassHeading() {

    readRawBytesOfMagneticData();
    // if failed
    convertRawBytesToIntMagneticData();
    convertThreeAxisIntMagneticDataToCompassHeading();
}
