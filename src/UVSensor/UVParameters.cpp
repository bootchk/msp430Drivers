


#include "UVParameters.h"

#include "UVCommands.h"

#include "UVSensor.h"  // errors



/*
 * A ParamSet command has an address embedded in it.
 */
UVCommands::UVCommand
UVParameters::buildParamSetCommandForAddress(unsigned int address) {
    // assert address is 6-bit
    // Set the seventh bit
    UVCommands::UVCommand result = static_cast<UVCommands::UVCommand>  ((1 << 6) + address);
    return result;
}



/*
 * Write value to indirectly addressed param array at address.
 */
unsigned int
UVParameters::set(unsigned char value, Parameter address) {
    unsigned int fail;

    /*
     * An optimization can be to set both Input and Command register in same transfer, since they are together.
     */

    fail = UVCommands::waitUntilSleep();
    if (fail) return fail;

    // Simple set register with the value for the parameter
    fail += UVCommands::setInput(value);

    UVCommands::UVCommand command = buildParamSetCommandForAddress(address);

    /*
     * This is a *send*, which waits for command to finish
     */
    fail += UVCommands::sendCommand(command);

    return fail;
}

