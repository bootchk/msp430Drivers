
#include "compass.h"
#include "mangler.h"
#include "compassInterface.h"





void Compass::turnPowerOn() { }

void Compass::turnPowerOff() { }

void Compass::reset() { }

void Compass::configureForLowPowerSingleRead() {
    CompassInterface::setLowPowerMode();
    CompassInterface::setSingleConversionMode();
}


unsigned int Compass::readContinuousCompassHeading() {
    // delegate
    return Mangler::readCompassHeading();
}

/*
 * Must read (which resets data ready), wait for data ready, then read data again.
 */
unsigned int Compass::readSingleCompassHeading() {

    configureForLowPowerSingleRead();

    Mangler::readAndDiscardToResetDataReady();

    while (not CompassInterface::isDataReady() );

    return readContinuousCompassHeading();
}

