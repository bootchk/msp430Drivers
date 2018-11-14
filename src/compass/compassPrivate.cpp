#include "compass.h"

#include <inttypes.h>


int16_t mag[3];

uint8_t rawMag[6];

void Compass::readRawBytesOfMagneticData() {


}


/*
 * Common endian-ness manipulation:
 * Put high and low order bytes into an int.
 *
 * This knows the order of bytes in a raw read:
 * As specified by the data sheet
 * x, y, z
 * low, high
 * I.E. consecutive register bytes are xlow, xhigh, ylow, ...
 */
void Compass::convertRawBytesToIntMagneticData() {
    // combine high and low bytes into signed 16-bit int
     mag[0] = (int16_t)(rawMag[1] << 8 | rawMag[0]);
     mag[1] = (int16_t)(rawMag[3] << 8 | rawMag[2]);
     mag[2] = (int16_t)(rawMag[5] << 8 | rawMag[4]);
}


/*
 * Concerns:
 * - tilt compensation (projection onto horizontal plane)
 * - units conversion (gauss counts into degrees)
 * - deviation of mag north from true geographic north
 * - calibration to adjust for local magnetic bias (metal nearby)
 *
 * The following implementation is the simplest.
 * It assumes we don't care about any of the concerns except units conversion.
 * Mainly, that the plane of the mounted compass chip is horizontal.
 */
/*
See Honeywell AN-203 application note

Direction (y>0) = 90 - [arcTAN(x/y)]*180/1
Direction (y<0) = 270 - [arcTAN(x/y)]*180/1
Direction (y=0, x<0) = 180.0
Direction (y=0, x>0) = 0.0
*/
unsigned int Compass::convertThreeAxisIntMagneticDataToCompassHeading() {
    unsigned int result;

    if (mag[1] == 0) {
        if (mag[0] < 0) result = 180;
        else result = 0;
    }
    else
    {
        if (mag[1] > 0) result = 90 - (arctan(mag[0]/mag[1])*180);
        else result = 270 - 0;
    }
    return result;
    // assert result in range [0, 360] i.e. a compass heading
}
