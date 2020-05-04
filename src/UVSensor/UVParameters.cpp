


#include "UVParameters.h"

#include "UVCommands.h"

#include "UVSensor.h"  // errors



/*
 * A ParamSet command has an address embedded in it,
 * and a bit B7 set for read or a bit B8 set for write.
 *
 * These do not enforce that address is 6-bit
 */
UVCommands::UVCommand
UVParameters::buildParamSetCommandForParameter(Parameter address) {
    //  0x80 + ((uint8_t)address & 0x3F);
    UVCommands::UVCommand result = static_cast<UVCommands::UVCommand>  (0x80 + address);
    return result;
}
UVCommands::UVCommand
UVParameters::buildParamQueryCommandForParameter(Parameter address) {
    //  0x80 + ((uint8_t)address & 0x3F);
    UVCommands::UVCommand result = static_cast<UVCommands::UVCommand>  (0x40 + address);
    return result;
}



#ifdef USE_RESPONSE1_FOR_VERIFY
/* Implement by comparing to RESPONSE1 register */
bool
UVParameters::verify(unsigned char value, Parameter address) {
    unsigned char writeBack[5];
    unsigned int fail;

    fail = UVCommands::getResponse1(writeBack);
    if (fail) return false;
    else return  (value == writeBack[0]);
}
#else
/* Implement by reading the param */
bool
UVParameters::verify(unsigned char value, Parameter address) {
    unsigned char readValue;
    unsigned int fail;

    fail = UVParameters::get(&readValue, address);
    if (fail) return false;
    else return  (value == readValue);
}
#endif


/*
 * Write value to indirectly addressed param array at address.
 */
unsigned int
UVParameters::set(unsigned char value, Parameter address) {
    unsigned int fail;

    // An optimization can be to set both Input and Command register in same transfer, since they are together.

    fail = UVCommands::waitUntilSleep(5);
    if (fail) return fail;

    // Simple set register with the value for the parameter
    fail += UVCommands::setInput(value);

    UVCommands::UVCommand command = buildParamSetCommandForParameter(address);

    // This is a *send*, which waits for command to finish

    fail += UVCommands::sendCommand(command);

    // Verify using the value the device wrote back
    if (not verify(value, address)) fail += UVSensor::DeviceError;

    return fail;
}


unsigned int
UVParameters::get(unsigned char *value, Parameter parameter) {
    unsigned int fail;
    UVCommands::UVCommand command;

    command = buildParamQueryCommandForParameter(parameter);

    fail = UVCommands::sendCommand(command);
    if (fail) return fail;

    fail += UVCommands::getResponse1(value);
    return fail;
}

