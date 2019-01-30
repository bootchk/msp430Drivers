
/*
 * Low level interface to stepper driver
 *
 * Where the implementation manipulates pins
 * I.E. hides access to pins via DriverLib
 */

class StepperDriver {
public:
    /*
        Unpower, to save energy.
        Switch Vcc to motor driver chip.
        Typically high-side switch (better than low-side switch i.e. switched ground.)

        Requires switch enabled high (typically a load switch, a PMOS with inverting control mosfet.)
        Caller must wait appropriate time defined by driver chip datasheet
        for time from powerup to ready to step.
        */
        static void turnPowerOn();
        static void turnPowerOff();
};
