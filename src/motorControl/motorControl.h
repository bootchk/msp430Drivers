/*
 * Understands control motor having a speed output
 */


class MotorControl {
public:
    static void initPins();

    static void startTurnCounter( int turnsToReach, int polePairs);
    static void stopTurnCounter();

    // Used by ISR
    static void handleInterrupt();
    static void setCountReachedFlag();

    //
    static bool wasCountReachedFlag();
};
