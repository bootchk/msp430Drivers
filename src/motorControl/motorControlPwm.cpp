


#include "motorControl.h"




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

