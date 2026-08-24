/*
Understands control motor having a speed output

Two separate modes:
    Catch first turn
    Catch a count of turns

The FG pin of the driver is high while motor off.
FG pin pulses low at the rate of commutation (not turns.)
FG pin of driver IC is open-drain, requires pullup.
FG pin stays high while motor is starting,
and goes low only after motor is turning in some sense stable.

Note that the MSP430 internal pullup is 20-50k ohms.
That doesn't seem to be enough to give a clean pulse to zero
on the FG pin of DRV10866.
Use a 4.7k ohm external resistor.
*/


class MotorControl {
public:
    static void initPinsForTurnCount();
    static void initPinsForSingleTurn();
    static void disableSingleTurnInterrupt();
    static void enableSingleTurnInterrupt();


    static void startTurnCounter( int turnsToReach, int polePairs);
    static void stopTurnCounter();

    // Used by ISR
    static void handleTimerInterrupt();
    static void handlePinInterrupt();
    static void setCountReachedFlag();

    //
    static bool wasCountReachedFlag();
};
