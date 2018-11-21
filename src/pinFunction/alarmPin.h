
/*
 * Understands alarm pin
 */


class AlarmPin {
public:
    static void configurePullupLoToHiInterrupt();
    static bool isConfigured();
    static void enableInterrupt();
    static void clearInterrupt();
    static bool isInterruptClear();
    static bool isHigh();
};
