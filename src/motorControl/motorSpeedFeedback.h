/*
Understands motor having a speed feedback from motor.
The desired speed is controlled.
This is the actual speed as fed back.

Two separate implementations:
    Use a Timer device to count turns
    Use a GPIO interrupt to count turns.

You can know if there are any turns, or the count of turns.
For some motors, the first pulse is after the motor
has turned a few revs.
For other motors, the motor may not have turned much.

The FG pin of the driver is high while motor off.
FG pin pulses low at the rate of commutation (not turns.)

FG pin stays high while motor is starting,
and goes low only after motor is turning in some sense stable.

FG pin of driver IC is open-drain, requires pullup.
Note that the MSP430 internal pullup is 20-50k ohms.
That doesn't seem to be enough to give a clean pulse to zero
on the FG pin of DRV10866.
Use a 4.7k ohm external resistor.
*/


class MotorSpeedFeedback{
public:
    // Use a Timer device to count turns
    static void initPinsForTurnCount();
    static void startTurnCounter( int turnsToReach, int polePairs);
    static void stopTurnCounter();
    // Used by ISR
    static void handleTimerInterrupt();


    // Use GPIO interrupt to count turns
    static void initPinsForTurnInterrupt();
    static void disableTurnInterrupt();
    static void enableTurnInterrupt();
    // Used by ISR
    static void handlePinInterrupt();

    
    static bool         didTurn();
    static unsigned int getTurnCount();
};
