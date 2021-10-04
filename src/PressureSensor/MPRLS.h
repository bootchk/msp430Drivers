

#include <stdint.h> // uint8_t

// I2C address
#define MPRLS_DEFAULT_ADDR (0x18)

#define MPRLS_READ_TIMEOUT (20)     ///< millis

// There are no RegisterAddress, only a "command" (i.e. a single register)
#define MPRLS_MEASURE_COMMAND (0xAA)

// Status bits
#define MPRLS_STATUS_POWERED (0x40)  // powered
#define MPRLS_STATUS_BUSY    (0x20)  // busy
#define MPRLS_STATUS_FAILED  (0x04)  // integrity fail
#define MPRLS_STATUS_MATHSAT (0x01)  // math saturation fail

#define MPRLS_STATUS_MASK (0b01100101) ///< Sensor status mask: only these bits are set

#define COUNTS_224 (16777216L)      ///< Constant: 2^24
#define PSI_to_HPA (68.947572932)   ///< Constant: PSI to HPA conversion factor




class MPRLS {
public:

  /*
  Adafruit_MPRLS(int8_t reset_pin = -1, int8_t EOC_pin = -1,
                 uint16_t PSI_min = 0, uint16_t PSI_max = 25,
                 float OUTPUT_min = 10, float OUTPUT_max = 90,
                 float K = PSI_to_HPA);
  */
    /*
    setup and initialize communication with the hardware
    No built-in delay, but one is required before reading.
    */
    void       begin(uint8_t i2c_addr = MPRLS_DEFAULT_ADDR);

    uint8_t    readStatus(void);


    /*
     Read pressure in units of PSI.
     Gage: relative to an open on the backside.
     Ambient pressure which is about 14 PSI
     */
    float      readPressure(void);

    /*
     Read 24 bits of raw pressure data.
     Returned in 32 bits, upper byte is zero if successful.
     On error, returns -1 ( all bits one)
     */
    uint32_t   readRawPressure(void);


private:

  // read 3 bytes of raw measurement data.
  // Returns -1 on error
  uint32_t readData(void);

};
