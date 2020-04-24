#include "UVCommands.h"


/*
 * Knows how to read UV sensor parameters.
 *
 * Knows that the parameters are one more level deep in the device.
 * Must write to the Input register, then form a new "set param" command, then write it to Command register.
 */
class UVParameters {

public:

    enum Parameter {
        ChannelList =         0x1,
        ADCConfig0 =          0x2,
        ADCSensitivity0 =           0x3,
        ADCPost0 =            0x4

    };

    static UVCommands::UVCommand buildParamSetCommandForAddress(unsigned int address);




    /*
     * Write <value> to <parameter>
     */
    static unsigned int set(unsigned char value, Parameter parameter);

};
