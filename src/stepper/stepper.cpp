#include "stepper.h"
#include <msp430.h>
#include <cassert>

#include "config.h"

/*
 Compile time define order of switching bits of IN/IN
 If this is not defined, then we brake between steps: both bits high briefly
 It doesn't matter for single stepping, but for manyStep, coasting may save energy?
 */
#define CoastBetweenSteps

/*
 Compile time definition of whether whole stepping is full or half power.
 Half stepping is a mix of full and half power.
 */
#define FullPower


// Define class data
int Stepper::currentStep = 0;
bool Stepper::isCoastingMode = false;
bool Stepper::isHighTorqueMode = false;




int Stepper::getStepNext(int direction)
{
    // require direction is +1 or -1
    return positiveModulo((currentStep + direction), 4);
    // ensure result is positive, is modulo 4, and one different from INcurrentStep
    // !!! currentStep need not be actual motor position.
}



void Stepper::reset()
{
    // state comprises all GPIO pin states, and class data
    turnOffDriver();
    turnOffCoils();

    currentStep = 0;// This may not match the physical position of rotor!
    isCoastingMode = false;
    isHighTorqueMode = false;

    configureIOPortForMotorControl();
}

void Stepper::configureIOPortForMotorControl()
{
    // Set direction of all port pins to output
    P1DIR |= MotorA1 | MotorA2 | MotorB1 | MotorB2 | DrivePowerSwitch;

    // Select function: all pins as digital IO
    // Bit clear.  Meaning of 0 is: IO function for pin
    // FR2433 has P1SEL0 and 1
    P1SEL0 &= ~(MotorA1 | MotorA2 | MotorB1 | MotorB2 | DrivePowerSwitch);

    // P1OUT is not set by this method
}

// TODO evacuate use of the port
// For now, if there is any other use of the pins, it may interfere with IO port settings

/*
 Caller must hold step an appropriate time for the desired speed.
 That is, these do not include a delay.
 */

void Stepper::stepToStep(int nextStep)
{
if (isHighTorqueMode)
    advanceStepFullPower(nextStep);
    // advanceStepWave(nextStep);
else
    advanceStepHalfPower(nextStep);
}

void Stepper::stepForward()
{   stepToStep(getStepNext(1));}
void Stepper::stepBackward()
{   stepToStep(getStepNext(-1));}

void Stepper::singleStepForward()
{
    stepForward();
    // coils are energized
    //TODO delay, enough to overcome inductance ??
    turnOffCoils();
    // coils are denergized
}

void Stepper::stepManyForward(int count, int speed)
{
    for (int i=0; i<count; i++)
    {
        // TODO direction
        Stepper::stepForward();

        // TODO delay according to speed
        //StepTime = foo
        //__delay_cycles(StepTime);
    }
}

/*
 * Implementation depends on driver chip?
 * All driver IN's low, which yields all driver OUT's floating (high impedance)
 */
void Stepper::turnOffCoils()
{
    P1OUT &= ~( MotorA1 | MotorA2 | MotorB1 | MotorB2); // bit clear
}

void Stepper::turnOnDriver()
{   P1OUT |= DrivePowerSwitch;}

void Stepper::turnOffDriver()
{   P1OUT &= ~DrivePowerSwitch;}




// private class methods

/*
 C modulo is really remainder and can return a negative number!
 Return positive number as defined by math modulo operator.
 */
int Stepper::positiveModulo(int a, int modulus)
{
    // Require modulus positive, not require a positive
    // if(modulusb < 0) //you can check for modulus == 0 separately and do what you want
    // return mod(-a, -b);
    int result = a % modulus;
    if (result < 0)
        result += modulus;
    return result;
}

void breakOpen(unsigned int breakBit)
{
    // bit clear
    P1OUT &= ~breakBit;
}

void makeClosed(unsigned int makeBit)
{
    // bit set
    P1OUT |= makeBit;
}

void breakThenMake(unsigned int makeBit, unsigned int breakBit)
{
    breakOpen(breakBit);
    /*
     * Assert both ends of coil shorted to ground, field dumps: coasting
     */
    makeClosed(makeBit);
}

void makeThenBreak(unsigned int makeBit, unsigned int breakBit)
{
// TODO
}

/*
 * bits off in any order
 */
void breakBits(unsigned int bit1, unsigned int bit2) {
    // bit clear
    P1OUT &= ~(bit1 & bit2);
}

void makeBits(unsigned int bit1, unsigned int bit2) {
    // bit set
    P1OUT |=  (bit1 & bit2);
}

/*
 * Clear breakBit and set makeBit.
 * Order of clearing and setting depends on coasting mode.
 */
void Stepper::energizeCoil(unsigned int breakBit, unsigned int makeBit)
{
    if (Stepper::isCoastingMode)
    {
        breakThenMake(breakBit, makeBit);
    }
    else
    {
        makeThenBreak(breakBit, makeBit);
    }
    // assert makeBit 1 high, breakBit 0 low
}

void Stepper::deenergizeCoil(unsigned int bit1, unsigned int bit2)
{
    if (Stepper::isCoastingMode)
    {
        // Both IN/IN low is both grounded, coasting
        breakBits(bit1, bit2);
    }
    else
    {
        // both high is both floating, braking
        makeBits(bit1, bit2);
    }
}


/*
 Forward and reverse current direction on coils A and B

 If this were many stepping:
 (but we are not, single stepping assures a bit we are clearing is already clear.)
 In both coils energized technique, a bit being cleared is already high.
 We clear first, so briefly (say 1uSec) both are low (coasting)
 instead of briefly both  high (which is braking).
 */
void Stepper::setAForward()
{   energizeCoil(MotorA2, MotorA1);}
void Stepper::setABackward()
{   energizeCoil(MotorA1, MotorA2);}
void Stepper::setBForward()
{   energizeCoil(MotorA2, MotorA1);}
void Stepper::setBBackward()
{   energizeCoil(MotorA1, MotorA2);}


void Stepper::setAOff() {
    deenergizeCoil(MotorA1, MotorA2);
}

void Stepper::setBOff()
{
    deenergizeCoil(MotorB1, MotorB2);
}


/*
 Whole step with both coils energized.
 Known as 'full step drive'.
 Both coils on gives maximum torque, but pulls twice the current.

 Note we don't know which direction we are turning
 so we can't just switch one pair (even though one is already configured properly)

 Requiring nextStep is adjacent to currentStep is not hard requirement:
 when starting, nextStep may not be adjacent to physical current step.
 Even if not adjacent, motor might turn.
 E.g. currentStep=0 to nextStep=2 might work.
 */
void Stepper::advanceStepFullPower(int nextStep)
{
    // require nextStep is positive, modulo 4
    // require nextStep is not currentStep and nextStep is adjacent to currentStep
    switch (nextStep)
    {
        case 0: setAForward(); setBForward(); break;    // 1010
        case 1: setABackward(); setBForward(); break; // 0110
        case 2: setABackward(); setBBackward(); break; //0101
        case 3: setAForward(); setBBackward(); break; //1001
        default:
        assert(false);
    }
    currentStep = nextStep;
    // ensure currentStep is INcurrentStep + 1
    // ensure both coils are driven in some direction
}

/*
 Turn the nearer coil (the one not changing from previous step) on first, delay short time, turn far coil on.
 TODO currently only works forward
 */
void Stepper::advanceStepWave(int nextStep)
{
    // require nextStep is positive, modulo 4
    // require nextStep is not currentStep and nextStep is adjacent to currentStep
    switch (nextStep)
    {
        case 0: setAForward(); __delay_cycles(500); setBForward(); break; // 1010
        case 1: setBForward(); __delay_cycles(500); setABackward(); break;// 0110
        case 2: setABackward(); __delay_cycles(500); setBBackward(); break;//0101
        case 3: setBBackward(); __delay_cycles(500); setAForward(); break;//1001
        default:
        assert(false);
    }
    currentStep = nextStep;
    // ensure currentStep is INcurrentStep + 1
    // ensure both coils are driven in some direction
}

/*
 Whole step with only one coil energized.
 Known as 'wave drive'.
 */
void Stepper::advanceStepHalfPower(int nextStep)
{
    // require nextStep is positive, modulo 4
    // require nextStep is not currentStep and nextStep is adjacent to currentStep
    switch (nextStep)
    {
        case 0: setAForward(); setBOff(); break;   // 1000
        case 1: setAOff(); setBForward(); break;// 0010
        case 2: setABackward(); setBOff(); break;// 0100
        case 3: setAOff(); setBBackward(); break;// 0001
        default:
        assert(false);
    }
    currentStep = nextStep;
    // ensure currentStep is INcurrentStep + 1
    // ensure at most one coil driven in some direction
}

