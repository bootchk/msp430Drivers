
/*
 * Bus slave addresses (serial: I2C or SPI)
 */



/*
 * 7-bit address
 * 0011100 if SDO pin is grounded
 * 0x1e  0b0011110 if SDO pin is high
 * Pololu breakout board pullups SDO pin high by default.
 */
#define LIS3MDLAddress 0x1e




/*
 * Abracon says 7-bit address is 0b1101001 (0x69)
 * But they say "0xD2/3", which includes the R/W bit which is transmitted AFTER the seven bit address
 * 0xD2 is 1101 0010 which shifted right one bit is 0110 1001 which is 0x69, the same thing
 */
#define AB0805BusAddress 0x69

// Alias
#define RTCBusAddress AB0805BusAddress


/*
 * VEML6075
 */
// 7-bit 0x10
//#define UVSensorBusAddress 0x10

/*
 * Si1133
 */
#define UVSensorBusAddress 0x55


