#include "UVSensor.h"

#include "UVCommands.h"
#include "UVParameters.h"


//#define ALT_PARAMS 1
#define UV_PARAMS 1
//#define WHITE_PARAMS 1

#ifdef ALT_PARAMS
/*
 * Zero decimation rate
 * mux is UV sensor, bits 4:0 => 11000
 * 0x18
 */
const unsigned char ADCConfig = 0x18;
// ADCSENSx defaults to one measurement
// Set to 128 measurements, time 48 uSec
const unsigned char ADCSensitivity = 0x71;

#elif defined(UV_PARAMS)
// short decimation 11 UV diode 11000 => 1111000 n0x78
const unsigned char ADCConfig = 0x78;

// 12.5 mSec HW_SENS
//const unsigned char ADCSensitivity = 0x09;
// 50 mSec HW_SENS
const unsigned char ADCSensitivity = 0x0b;
// 50 mSec HW_SENS and High range
//const unsigned char ADCSensitivity = 0x8b;

// 24-bit result
const unsigned char ADCPost = 0x40;
#else
/*
 * White light sensor
 */
// decimation 2 and didode is large white
// const unsigned char ADCConfig = 0x4d;
// decimation 0 and dido is large white
const unsigned char ADCConfig = 0x0d;
// HW_SENS = 50ms, and 2 reads
// const unsigned char ADCSensitivity = 0x1b;
// HW_SENS = 50ms and 1 read
//const unsigned char ADCSensitivity = 0xb;
// HW_SENS = 24uSec and 1 read
const unsigned char ADCSensitivity = 0x0;
// 24-bit result
// Default is 16-bit result, no shift, no thresholds
const unsigned char ADCPost = 0x40;
#endif

/*
 * Each channel has four params, but we accept most of the reset defaults
 */
unsigned int
setParametersForUVRead() {
    unsigned int fail;

    fail = UVParameters::set(ADCConfig,       UVParameters::Parameter::ADCConfig0);
    fail += UVParameters::set(ADCSensitivity, UVParameters::Parameter::ADCSensitivity0);
    fail += UVParameters::set(ADCPost,        UVParameters::Parameter::ADCPost0);

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

    // We just reset device, no need to reset command counter
    // fail = UVCommands::sendCommandResetCommandCounter();

    fail = setParametersForUVRead();
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
unsigned int measureUV(long* result) {
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
UVSensor::readSingleUVA(long *UVResult) {
    unsigned int fail;

    fail = reset();
    fail += configureToReadUV();
    fail += measureUV(UVResult);
    return fail;
}
