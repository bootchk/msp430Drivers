

// The constant a VEML6075 chip returns as identifier from register that holds ID
// ID register is 16 bit but only lower byte holds non-zero data
const unsigned int VEML6075Identifier = 0x26;


enum UVSensorAddress {
    UVABytes =        0x07,
    Identifier =      0x0C,
};
