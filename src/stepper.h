/*
  Drive stepper motor:
  - bipolar (two coils)
  - using 4 wires
  - IN/IN protocol to a driver IC
  - always two coils energized (among other techniques)
  
  Simple API:
  - single steps, forward or back.
  - turnOffCoils: set driver inputs low so driver outputs float: H-bridge, two mosfets off)
  - turn driver power off and on (for low power)
  
  TODO
  half stepping (one coil energized sometimes)
  save current step to flash in case mcu gets unpowered
  stepMany
  save currentStep to flash
  
  Compatible with TI DRV8835 part.
  
  Stateless except for current step.  
  Does not know speed or which direction it was moving.

  Std sequence of control signals, see below.
 
  Low memory usage:  inlined methods and class methods.
  All data and methods belong to class:  No instance!  Do not use 'this'!
  Invoke as Stepper::stepForward()

  Typical call sequences:
    Single stepping:
       reset(), turnDriverOn(), stepForward(), wait, turnOffCoils(), turnDriverOff(), 
          turnDriverOn(), stepBackward(), wait, ...
    Many single steps()
       reset(), turnDriverOn(), stepForward(), wait, stepForward(), turnOffCoils(), turnDriverOff(), ....
   Many stepping()
       reset(), turnDriverOn(), manyStepForward(steps, speed), ...
    
    
  All port positions (bits of a port) are compile time defined as macros.
  Needs 5 port bits
  
  On DRV8835 driver, pin 11 mode is grounded low: IN/IN protocol
  On Polulu breakout board, J1 pin 7 is mode.
  
  Timing:
  driver on to first step: 40uSec (DRV8835 datasheet)
  IN toggle to OUT valid:  ~300nsec (DRV8835 datasheet)
  step to turnOffCoils(): ? depends on motor and current speed of rotor
*/

#include <msp430.h>

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




class Stepper {
private:
    // class data

    // Declaration only.  See near end of file where one instance defined.
    static int currentStep;// ISO forbids initialization


public:

    
    /*
    When turned on, the coils are not energized.
    They are NOT set energized to currentStep, but to nextStep.
    Reluctance of rotor holds rotor at position of currentStep (not energized coils)
    when driver is off.
    It is always possible that rotor is NOT at the current step.
    It is also possible that in this case, one stepping action would not step the motor.
    But a sequence of stepping actions should eventually step the motor.
    */
    


    static void reset();
    
    static void configureIOPortForMotorControl();
    
    
    // TODO evacuate use of the port
    // For now, if there is any other use of the pins, it may interfere with IO port settings
    
    
    /*
    Caller must hold step an appropriate time for the desired speed.
    That is, these do not include a delay.
    */

    /*
     *  Hides full (two coils) or half (one coil) power.
     */
    static void stepToStep(int nextStep);

    /*
     * Convenience functions.
     */

    /*
     * Step from current step,
     * Leave coils energized (holding torque).
     * Use when you want to control the delay between steps.
     */
    static void stepForward();
    static void stepBackward();

    /*
     * Step from current step.
     * Leave coils off (only detent torque).
     */
    static void singleStepForward();
    static void singleStepBackward();
   

    static void stepManyForward(int count, int speed);
    
    /*
    After a step, you may turnOffCoils.
    turnOffCoils leaves the motor outputs floating (coasting), and motor may overshoot?
    Braking not implemented (both controls high, both outputs shunted to ground.
    Neither coasting nor braking uses current at the motor?
    */
    static void turnOffCoils();
    
    
    /*
    Unpower driver to save energy.
    Switch Vcc to motor driver.
    Typically high-side switch (better than low-side switch i.e. switched ground.)

    Requires switch enabled high (typically a load switch, a PMOS with inverting control mosfet.)
    Caller must wait appropriate time defined by driver chip datasheet
    for time from powerup to ready to step.
    */
    static void turnOnDriver();
    static void turnOffDriver();
    
    
  private:
    
    // private class methods
    
    /*
    C modulo is really remainder and can return a negative number!
    Return positive number as defined by math modulo operator.
    */
    static int positiveModulo(int a, int modulus) {
        // Require modulus positive, not require a positive
        // if(modulusb < 0) //you can check for modulus == 0 separately and do what you want
        // return mod(-a, -b);   
        int result = a % modulus;
        if(result < 0) result += modulus;
        return result;
    }
    
    
    /*
     * Get step in direction from current step.
     *
     * If the motor is not actually at current step,
     * results are undefined.
     */
    static int getStepNext(int direction);


    
    /*
    Forward and reverse current direction on coils A and B
    
    If this were many stepping:  
    (but we are not, single stepping assures a bit we are clearing is already clear.)
    In both coils energized technique, a bit being cleared is already high.
    We clear first, so briefly (say 1uSec) both are low (coasting)
    instead of briefly both  high (which is braking).
    */
    static void setAForward();
    static void setABackward();
    static void setBForward();
    static void setBBackward();
    
    
    #ifdef CoastBetweenSteps
    // Both IN/IN low is coasting
    static void setAOff();
    static void setBOff();
    #else
    // Both IN/IN high is braking
    static void setAOff();
    static void setBOff();
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
static void advanceStepFullPower(int nextStep);

/*
Turn the nearer coil (the one not changing from previous step) on first, delay short time, turn far coil on.
TODO currently only works forward
*/
static void advanceStepWave(int nextStep);
/*
 Whole step with only one coil energized.
 Known as 'wave drive'.
*/
static void advanceStepHalfPower(int nextStep);

};


