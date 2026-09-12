


#include "motorControl.h"


#include "../i2c/i2c.h"

// TODO this is mockup to test will fit in memory

// Write to a slave register that controls speed
void
MotorControl::start(unsigned int motorDutyCycle)
{
    unsigned char buffer[3];

    I2C::configureMaster( 1, true);
    I2C::write(1, buffer, 3);
    I2C::unconfigureMaster();
    // ensure pins are not configured for I2C
}

void 
MotorControl::stop()
{

}

void 
MotorControl::toggleDirection()
{

}

