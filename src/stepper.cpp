
#include "stepper.h"
#include <msp430.h>
#include <cassert>





// Compile time definition of mcu pins connected to driver IN pins
// For example, to pins 3,4,5,6 of MSP430F2012 connected to pins AIN1, AIN2, BIN1, BIN2 of DRV8835
// BIT0 is usually an LED on a launchpad target board
#define MotorA1 BIT1    // Coil A
#define MotorA2 BIT2
#define MotorB1 BIT3    // Coil B
#define MotorB2 BIT4

// to gate of mosfet power switch that switches Vcc to driver
#define DrivePowerSwitch BIT5

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


// One definition of class data
//int currentStep = 0;



int Stepper::getStepNext(int direction)
{
    // require direction is +1 or -1
    return positiveModulo((currentStep + direction), 4);
    // ensure result is positive, is modulo 4, and one different from INcurrentStep
    // !!! currentStep need not be actual motor position.
}
    




  
void Stepper::reset()
{
    // reset state
    turnOffDriver();
    turnOffCoils();
    currentStep = 0;// This may not match the physical position of rotor!
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

    // Equivalent in Wiring
    // pinMode(3, OUTPUT); ... pinMode(6, OUTPUT);
}
    
    
    // TODO evacuate use of the port
    // For now, if there is any other use of the pins, it may interfere with IO port settings
    
    
    /*
    Caller must hold step an appropriate time for the desired speed.
    That is, these do not include a delay.
    */


void Stepper::stepToStep(int nextStep) {
        #ifdef FullPower
            advanceStepFullPower(nextStep);
            // advanceStepWave(nextStep);
 	    #else
            advanceStepHalfPower(nextStep); 
        #endif
    }

void Stepper::stepForward() { stepToStep(getStepNext(1)); }
void Stepper::stepBackward() { stepToStep(getStepNext(-1)); }
   

void Stepper::stepManyForward(int count, int speed) {
        for (int i=0; i<count; i++) {
            // TODO direction
            Stepper::stepForward();
            
            // TODO delay according to speed
            //StepTime = foo
            //__delay_cycles(StepTime);
        }
  }
    
/*
 After a step, you may turnOffCoils.
 turnOffCoils leaves the motor outputs floating (coasting), and motor may overshoot?
 Braking not implemented (both controls high, both outputs shunted to ground.
 Neither coasting nor braking uses current at the motor?
 */
void Stepper::turnOffCoils()
{
    // All driver IN's low, which yields all driver OUT's floating
    P1OUT &= ~( MotorA1 | MotorA2 | MotorB1 | MotorB2);// bit clear
}
    
    
    /*
    Unpower driver to save energy.
    Switch Vcc to motor driver.
    Typically high-side switch (better than low-side switch i.e. switched ground.)

    Requires switch enabled high (typically a load switch, a PMOS with inverting control mosfet.)
    Caller must wait appropriate time defined by driver chip datasheet
    for time from powerup to ready to step.
    */
void Stepper::turnOnDriver() { P1OUT |= DrivePowerSwitch; }
void Stepper::turnOffDriver() { P1OUT &= ~DrivePowerSwitch; }
    
    
    
    // private class methods

/*
 C modulo is really remainder and can return a negative number!
 Return positive number as defined by math modulo operator.
 */
int positiveModulo(int a, int modulus)
{
    // Require modulus positive, not require a positive
    // if(modulusb < 0) //you can check for modulus == 0 separately and do what you want
    // return mod(-a, -b);
    int result = a % modulus;
    if (result < 0)
        result += modulus;
    return result;
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
{  // 10
    // Wiring equivalent: digitalWrite(3, HIGH); digitalWrite(4, LOW);
#ifdef CoastBetweenSteps
    P1OUT &= ~MotorA2; // bit clear
    P1OUT |= MotorA1;// bit set
#else
    P1OUT |= MotorA1; // bit set
    P1OUT &= ~MotorA2;// bit clear
#endif
}
    
void Stepper::setABackward()
{ // 01
    // Wiring equivalent: digitalWrite(3, LOW); digitalWrite(4, HIGH);
#ifdef CoastBetweenSteps
    P1OUT &= ~MotorA1; // bit clear
    P1OUT |= MotorA2;// bit set
#else	// Brake
    P1OUT |= MotorA2; // bit set
    P1OUT &= ~MotorA1;// bit clear
#endif
}

void Stepper::setBForward()
{  // 10
    //  Wiring equivalent: digitalWrite(5, HIGH); digitalWrite(6, LOW);
#ifdef CoastBetweenSteps
    P1OUT &= ~MotorB2; // bit clear
    P1OUT |= MotorB1;// bit set
#else	// Brake
    P1OUT |= MotorB1; // bit set
    P1OUT &= ~MotorB2;// bit clear
#endif
}

void Stepper::setBBackward()
{ // 01
    // Wiring equivalent: digitalWrite(5, LOW); digitalWrite(6, HIGH);
#ifdef CoastBetweenSteps
    P1OUT &= ~MotorB1; // bit clear
    P1OUT |= MotorB2;// bit set
#else	// Brake
    P1OUT |= MotorB2; // bit set
    P1OUT &= ~MotorB1;// bit clear
#endif
}

#ifdef CoastBetweenSteps
// Both IN/IN low is coasting
void Stepper::setAOff()
{
    P1OUT &= ~(MotorA1 | MotorA2);
} // bit clear
void Stepper::setBOff()
{
    P1OUT &= ~(MotorB1 | MotorB2);
} // bit clear
#else
// Both IN/IN high is braking
void Stepper::setAOff()
{   P1OUT |= MotorA1 | MotorA2;} // bit set
void Stepper::setBOff()
{   P1OUT |= MotorB1 | MotorB2;} // bit set
#endif
    

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
    switch (nextStep) {
      case 0: setAForward();  setBForward();  break;    // 1010
      case 1: setABackward(); setBForward();  break;    // 0110
      case 2: setABackward(); setBBackward(); break;    //0101
      case 3: setAForward();  setBBackward(); break;    //1001
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
    switch (nextStep) {
      case 0: setAForward();  __delay_cycles(500); setBForward();  break;    // 1010
      case 1: setBForward(); __delay_cycles(500);  setABackward(); break;    // 0110
      case 2: setABackward(); __delay_cycles(500); setBBackward(); break;    //0101
      case 3: setBBackward(); __delay_cycles(500); setAForward();  break;    //1001
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
    switch (nextStep) {
      case 0: setAForward();  setBOff();      break;   // 1000
      case 1: setAOff();      setBForward();  break;   // 0010
      case 2: setABackward(); setBOff();      break;   // 0100
      case 3: setAOff();      setBBackward(); break;   // 0001
      default:
        assert(false);
    }
    currentStep = nextStep;
    // ensure currentStep is INcurrentStep + 1
    // ensure at most one coil driven in some direction
}




// One definition of class data
int Stepper::currentStep = 0;

