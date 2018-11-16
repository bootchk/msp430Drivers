
#include "compass.h"
#include "mangler.h"

void Compass::turnPowerOn() { }

void Compass::turnPowerOff() { }

void Compass::reset() { }

unsigned int Compass::readCompassHeading() {
    // delegate
    return Mangler::readCompassHeading();
}
