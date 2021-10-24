/*
 * Class that knows the state of the chip
 * AND the pins that control it.
 *
 * After a sleep with disable,
 * and when using microstepping,
 * the motor state and the chip state may disagree,
 * since the motor will assume a detent position
 * and the chip will assume the home position.
 */

#pragma once


/*
 * Size of a microstep.
 * Value is microsteps per detent step.
 *
 * A detent is where the motor stops if not driven
 * Where the residual magnetism stops the rotation.
 *
 * Some driver chips and boards allow step size to be set at runtime.
 * Whether the board does  (whether hardwired to a specific step size)
 * is defined in board.h, conditional compilation.
 */
enum class StepSizeMode {
    Full = 1,
    Half = 2,
    Quarter = 4
};


enum class MotorDirection {
    Forward,
    Backward
};

enum class MotorSpeed {
    Max,
    Half,
    Quarter,
    Slow
};



class IndexerChipState {

public:
    static void           setDirection(MotorDirection);
    static MotorDirection getDirection();

    static unsigned int   microStepsPerDetentStep();

    /*
     * The state of the GPIO driving the "coil enable" pin of the chip.
     * If the chip is unpowered or sleep, then this is not the state of the coils,
     * i.e. there can't be any power on the chip output to the coils.
     */
    static bool           isCoilsEnabled();
    static void           enableCoils(bool);



    static void rememberMotorStep();

    /*
     * When you call this, you assert that motor step is in sync with given step.
     * Usually, you have wake() so that driver is on home step
     * and you have called microstep() through at least one cycle.
     */
    static void setMicrostepState(unsigned int);

    /*
     * Advance shadow state from home step to current step without energizing coils.
     * This disables the driver outputs while stepping, to advance DriverChip internal state w/o moving motor.
     */
    static void restoreDriverToMotorStep();
};
