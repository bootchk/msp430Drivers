/*
Understands motor control.

This hides the type of motor and the driver IC.

The motor may be:
   DC and the driver IC a simple low side switch.
   BLDC and the driver IC controlled by PWM and DIR pins
   BLDC and the driver IC controlled by I2C.
   
Even for DC motor, we may PWM the low side switch.

For BLDC and direct control of pins, we PWM the driver pin.
Some driver ICs do not have a DIR pin.

For BLDC and I2C interface, we write the duty cycle value
to a register of the slave.

A fully integrated BLDC has mosfets driving phases.

TODO: can we switch back and forth between direct and I2C modes
on some driver IC e.g. AMT49400
*/


class MotorControl{
public:
    static void start(unsigned int motorDutyCycle);
    static void stop();

    static void toggleDirection();

    // TODO if the driver IC is load switched.
    // void powerDown();
};

