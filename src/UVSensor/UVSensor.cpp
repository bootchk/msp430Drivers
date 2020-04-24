#include "UVSensor.h"

#include "UVCommands.h"
#include "UVParameters.h"



/*
 * Each channel has four params, but we accept most of the reset defaults
 */
unsigned int
setParametersForUVRead() {
    unsigned int fail;

    // Zero decimation rate, mux is UV sensor, bits 4:0 => 11000
    fail = UVParameters::set(0x18, UVParameters::Parameter::ADCConfig0);

    // ADCSENSx defaults to one measurement
    // Set to 126 measurements, time 48 uSec
    fail += UVParameters::set(0x71, UVParameters::Parameter::ADCSensitivity0);

    // Omit ADCPOSTx     Default is 16-bit result, no shift, no thresholds
    // Omit MEASCONFIGx

    // TODO not sure this is necessary
    // The IRQ bit will be set, but no interrupt on pin unless this is enabled?
    fail += UVCommands::enableIRQChannelZero();

    return fail;
}


unsigned int
configureChannelForUVRead() {
    // 0b1 is BIT0 is channel 0 select
    return UVParameters::set(1, UVParameters::Parameter::ChannelList);
}





/*
 * Configure device to sample UV
 *
 * Returns non-zero on error.
 */
unsigned int
configureToReadUV() {
    unsigned int fail;

    fail = UVCommands::sendCommandResetCommandCounter();
    fail += setParametersForUVRead();
    fail += configureChannelForUVRead();
    return fail;
}



/*
 * Measure UV.
 *
 * Require configured to read UV on channel 0.
 *
 * Returns non-zero on error.
 */
unsigned int measureUV(unsigned int * result) {
    unsigned int fail;

    fail = UVCommands::sendCommandForceRead();

    UVCommands::sleepWhileReadingChannel();

    fail += UVCommands::tryChannelZeroMeasureComplete();

    fail += UVCommands::getUVSample(result);
    return fail;
}



unsigned int
UVSensor::reset(void) {
    unsigned int fail;

    UVCommands::sleepWhileBooting();
    fail = UVCommands::sendCommandReset();
    UVCommands::sleepWhileResetting();
    return fail;
}




bool
UVSensor::isSane() {
    const unsigned char UVSensorID = 0x33;
    unsigned char ID;

    bool fail = UVCommands::getID(&ID);
    return (ID == UVSensorID) and (fail == 0);
}



unsigned int
UVSensor::readSingleUVA(unsigned int *UVResult) {
    unsigned int fail;

    fail = reset();
    fail += configureToReadUV();
    fail += measureUV(UVResult);
    return fail;
}
