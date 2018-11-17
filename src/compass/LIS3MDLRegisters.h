
// The constant a LIS3MDL chip returns as identifier from register that holds ID
const unsigned int LIS3MDLIdentifier = 0x3D;



enum class CompassAddress : unsigned char {

    Identifier =         0x0F,
    Control3 =           0x22,
    Status =             0x27,
    MagXYZBytes =        0x28,
};

/*
          CTRL_REG1   = 0x20,
          CTRL_REG2   = 0x21,
          CTRL_REG3   = 0x22,
          CTRL_REG4   = 0x23,
          CTRL_REG5   = 0x24,

          STATUS_REG  = 0x27,
          OUT_X_L     = 0x28,
          OUT_X_H     = 0x29,
          OUT_Y_L     = 0x2A,
          OUT_Y_H     = 0x2B,
          OUT_Z_L     = 0x2C,
          OUT_Z_H     = 0x2D,
          TEMP_OUT_L  = 0x2E,
          TEMP_OUT_H  = 0x2F,
          INT_CFG     = 0x30,
          INT_SRC     = 0x31,
          INT_THS_L   = 0x32,
          INT_THS_H   = 0x33,
*/
