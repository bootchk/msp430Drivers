


#include "motorControl.h"




// motor control is via a GPIO pin to a load switch that switches ground
// of the motor.
// But we still duty cycle the load switch

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

