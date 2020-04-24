

/*
 * The constant an I2C returns as identifier from register that holds ID
 *
 * ID register is 16 bit but only lower byte holds non-zero data.
 * The lower byte is streamed first?
 *
 */

//#define USE_VEML6075 1
#define USE_SI11335 1

#ifdef USE_VEML6075


const unsigned int UVSensorID = 0x26;


enum UVSensorAddress {
    UVABytes =        0x07,
    Identifier =      0x0C,
};

#else


// Moved deeper down


#endif
