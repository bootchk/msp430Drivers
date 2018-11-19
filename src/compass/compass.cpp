
#include "compass.h"
#include "mangler.h"
#include "compassInterface.h"





void Compass::turnPowerOn() { }

void Compass::turnPowerOff() { }

void Compass::reset() { }


bool Compass::isSane() { return CompassInterface::isReadable(); }


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

    // A read clears the data ready bit
    Mangler::readAndDiscardToResetDataReady();

    // Configuring wakes up the sensing mechanism and starts a computation
    configureForLowPowerSingleRead();

    while (not CompassInterface::isDataReady() );

    return readContinuousCompassHeading();
}

