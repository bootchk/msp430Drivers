
/*
 * Abstract SYSRSTIV register
 */

class ResetReason {
public:
    /*
     * Clear all the reset reasons.
     * Return true if any are "reset from low power x.5"
     * i.e. if the system was powered down previously and the reset is from an interrupt that wakes.
     */
    static bool isResetAWakeFromSleep();
};
