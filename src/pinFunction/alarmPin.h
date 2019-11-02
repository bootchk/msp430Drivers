
/*
 * Understands alarm pin
 */


class AlarmPin {
public:
    static void configurePullupLoToHiInterrupt();
    static bool isConfigured();

    static void enableInterrupt();
    static bool isEnabledInterrupt();
    static void clearInterrupt();
    static bool isInterruptClear();

    static bool isHigh();
};
