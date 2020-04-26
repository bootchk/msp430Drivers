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
        ADCSensitivity0 =     0x3,
        ADCPost0 =            0x4

    };

    static UVCommands::UVCommand buildParamSetCommandForParameter(Parameter address);
    static UVCommands::UVCommand buildParamQueryCommandForParameter(Parameter address);

    // Write <value> to <parameter>
    static unsigned int set(unsigned char value, Parameter parameter);
    // Read <value> to <parameter>
    static unsigned int get(unsigned char *value, Parameter parameter);

    /*
     * Return whether <parameter> has <value>
     */
    static bool verify(unsigned char value, Parameter parameter);
};
