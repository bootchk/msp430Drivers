


#include "motorControl.h"


// Choose one.  TODO move to board.h
//#define MOTOR_CONTROL_IS_DIRECT
#define MOTOR_CONTROL_IS_I2C


#if defined(MOTOR_CONTROL_IS_DIRECT)

// motor control is via a PWM and DIR pin of the driver IC
#include "../PWM/PWM.h"

void 
MotorControl::start(unsigned int motorDutyCycle)
{   
    PWM::turnOn(motorDutyCycle);
}

void 
MotorControl::stop()
{
    PWM::turnOff();
}

void 
MotorControl::toggleDirection()
{

}

#elif defined(MOTOR_CONTROL_IS_I2C)

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

#else
#warning "motor control method not defined"
#endif