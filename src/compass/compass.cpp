
#include "compass.h"
#include "mangler.h"
#include "compassInterface.h"





void Compass::turnPowerOn() { }

void Compass::turnPowerOff() { }

void Compass::reset() { }


bool Compass::isSane() { return CompassInterface::isReadable(); }

unsigned char Compass::readStatus() { return CompassInterface::readStatus(); }


void Compass::configureForLowPowerSingleRead() {
    CompassInterface::setLowPowerMode();
    CompassInterface::setSingleConversionMode();
}


unsigned int Compass::readContinuousCompassHeading() {
    // delegate
    return Mangler::readCompassHeading();
}

/*
 * A higher level protocol with these steps:
 * - read (which resets data ready)
 * - start a conversion (configure to single read mode)
 * -  wait for data ready
 * - read data again.
 */
unsigned int Compass::readSingleCompassHeading() {

    // A read clears the data ready bit
    Mangler::readAndDiscardToResetDataReady();

    // Configuring wakes up the sensing mechanism and starts sensing and computation
    configureForLowPowerSingleRead();

    while (not CompassInterface::isDataReady() );

    return readContinuousCompassHeading();
}

