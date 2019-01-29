/*
  Drive stepper motor:
  - bipolar (two coils)
  - using 4 GPIO for control, plus one for power on
  - IN/IN protocol to a driver IC
  
  Simple API:
  - single steps, forward or back.
  - forward or backward at a speed TODO
  - turnOffCoils: set driver inputs low so driver outputs float: H-bridge, two mosfets off)
  - turn driver power off and on (for low power)
  
  Design features:
  - clarity instead of performance
  - depends on platform independent layer (DriverLib) TODO

  TODO
  half stepping (one coil energized sometimes)
  save current step to flash in case mcu gets unpowered
  stepMany
  save currentStep to flash
  
  Compatible with TI DRV8835 part.
  
  State
  - current step.
  - modes
  State is persistent for FR chips in MSP430 family

  Modes
  - coasting or  braking
  - one coil or two coil driving
  Direction moving is not state.

  Std sequence of control signals, see below.
 
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
    
    
  All port positions (bits of a port) are compile time defined by macros.
  Needs 5 port bits.
  Hardcoded to PORT1
  
  On DRV8835 driver, pin 11 mode is grounded low: IN/IN protocol
  On Polulu breakout board, J1 pin 7 is mode.
  
  Timing:
  driver on to first step: 40uSec (DRV8835 datasheet)
  IN toggle to OUT valid:  ~300nsec (DRV8835 datasheet)
  step to turnOffCoils(): ? depends on motor and current speed of rotor
*/

#include <msp430.h>



typedef struct {
    int port;
    int pin;
} MotorCoilEnd;




class Stepper {
private:
    // class data

    // Declaration only.
    static int currentStep;// ISO forbids initialization

    /*
     Order of switching bits of IN/IN.
     Coasting: both bits low, shorted to ground and mag field collapses.
     Not coasting is braking: both bits high, ends of coil high impedance and mag field persists.

     ??? It doesn't matter for single stepping, but for manyStep, coasting may save energy?
     */
    static bool isCoastingMode;

    /*
     Whether whole stepping is full (two coils) or half (one coil) power.
     Half stepping is a mix of full and half power.
     */
    static bool isHighTorqueMode;

public:

    /*
     * Modes
     */
    static void setCoastingMode(bool);
    static void setHighTorqueMode(bool);
    
    /*
    When turned on, the coils are not energized.
    They are NOT set energized to currentStep, but to nextStep.
    Reluctance of rotor holds rotor at position of currentStep (not energized coils)
    when driver is off.
    It is always possible that rotor is NOT at the current step.
    It is also possible that in this case, one stepping action would not step the motor.
    But a sequence of stepping actions should eventually step the motor.
    */

    /*
     * Reset driver software, GPIO interface, and turn off driver chip.
     */
    static void reset();
    
    static void configureIOPortForMotorControl();
    
    
    // TODO release use of GPIO
    // For now, if there is any other use of the pins, it may interfere with IO port settings
    
    
    /*
    Caller must hold step an appropriate time for the desired speed.
    That is, these do not include a delay.
    */

    /*
     *  Hides high torque, full (two coils) or half (one coil) power.
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
    turnOffCoils leaves the motor outputs floating (braking),
    or grounded (coasting) according to coasting mode.
    Neither coasting nor braking uses current at the motor?
    */
    static void turnOffCoils();
    
    
    
    

  private:
    
    // private class methods
    
    /*
    C modulo is really remainder and can return a negative number!
    Return positive number as defined by math modulo operator.
    */
    static int positiveModulo(int a, int modulus);
    
    
    /*
     * Get step in direction from current step.
     *
     * If the motor is not physically at current step,
     * movement of motor when driven is unpredictable.
     */
    static int getStepNext(int direction);


    /*
     * Energize coil defined by bits.
     * end1Bit low, end2Bit high
     */
    static void energizeCoil(const MotorCoilEnd end1Bit, const MotorCoilEnd end2Bit);
    static void deenergizeCoil(const MotorCoilEnd end1Bit, const MotorCoilEnd end2Bit);
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
    
    
   /*
    * Denergize coil.
    *
    * Exist two alternatives:
    * - Both IN/IN low is coasting ( magnetic field collapses quickly, dumped to ground)
    * - Both IN/IN high is braking ( field collapse slowly, dumped to Vcc)
    * ??? Or are the FET float?
    */
    static void setAOff();
    static void setBOff();


    

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


