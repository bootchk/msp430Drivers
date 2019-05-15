
/*
 * An LED that also can sense light.
 *
 * pSideLED  ->|-  nSideLED
 *
 * Forward biased (lit) has pSide high, nSide low
 *
 * States:
 *    Unconfigured
 *    Off       LED dark            low  pSideLED  ->|-  nSideLED low
 *    On        LED lit             high pSideLED  ->|-  nSideLED low
 *    Reversed  LED reversed biased low  pSideLED  ->|-  nSideLED high
 *    Measuring LED                 low  pSideLED  ->|-  nSideLED input
 */
/*
 * Choice of implementation: see .cpp
 * I found that it takes a long time to detect extreme dark
 * (doesn't take long to detect moderate dark).
 * The sleeping implementation uses less power.
 * So if you want to detect extreme dark, and want less power usage, use the sleeping implementation.
 *
 * When implementation uses sleeping,
 * requires ISR for both GPIO and Counter, that simply clear their flags and exit sleep.
 * These ISR's are NOT in the project (but examples are in /test)
 * and are usually in a higher project.
 *
 * When implementation uses sleeping,
 * some methods (e.g. measureLight) may not return for seconds, when it is dark!!!!
 */





class LEDAndLightSensor {

private:

    static void toReversedFromOff();

    static void toMeasuringFromReversed();

    static unsigned int measureCapacitanceDischargeIteratively();
    static unsigned int measureCapacitanceDischargeSleeping();

    static void toOffFromMeasuring();

    static bool isLow();
    static void enableLowInterrupt();
    static void disableLowInterrupt();

public:
    // Public so ISR can clear it
    static void clearInterrupt();

    /*
     * LED function
     */

    /*
     * Initialization
     */
    static void toOffFromUnconfigured();

    static void toOnFromOff();

    static void toOffFromOn();



    /*
     * Light sensor function
     */
    /*
     * Assuming in light condition,
     * measureLight and store the result as a calibrated reference.
     */
    static void calibrateInLightOrReset();

    /*
     * Requires in Off state, and ensures in Off state.
     *
     * Return value indicates light level.
     * Low value means high illumination to LED.
     */
    static unsigned int measureLight();

    /*
     * Understands what measurements are dark.
     * WAS in relation to a constant.
     * NOW in relation to calibrated reference value.
     *
     * Requires prior call to calibrateInLightOrReset
     */
    static bool isNightDark();
    static bool isTwilightDark();
};
