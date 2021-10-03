

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

  bool       begin(uint8_t i2c_addr = MPRLS_DEFAULT_ADDR);  // , TwoWire *twoWire = &Wire);
  uint8_t    readStatus(void);
  float      readPressure(void);
  uint32_t   readRawPressure(void);

  //uint8_t lastStatus; /*!< status byte after last operation */


private:

  // read 3 bytes of raw measurement data.
  // Returns -1 on error
  uint32_t readData(void);

/*
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
  int8_t _reset, _eoc;
  uint16_t _PSI_min, _PSI_max;
  uint32_t _OUTPUT_min, _OUTPUT_max;
  float _K;
  */
};
