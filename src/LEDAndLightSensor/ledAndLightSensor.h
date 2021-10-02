
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

    /*
     * Reverse bias.
     * This will charge capacitance of LED, and N pin will be high.
     */
    static void toReversedFromOff();

    /*
     * Change N pin to an input.
     * It was high, but will immediately start discharging capacitance to low.
     * There is a race to enable interrupt before sleep.
     */
    static void toMeasuringFromReversed();

    static unsigned int measureCapacitanceDischargeIteratively();
    static unsigned int measureCapacitanceDischargeSleeping();

    static void toOffFromMeasuring();

    static bool isLEDNPinLow();

    /*
     * Enable interrupt of kind: high-to-low transition.
     * GPIO interrupts are always edge triggered.
     */
    static void enableHighToLowInterruptFromLEDNPin();
    // Disable any interrupt kind
    static void disableLEDNPinInterrupt();

public:
    /*
     * Public so ISR can clear it.
     *
     * Depending on the interrupt edge, this does not guarantee that the flag remains clear.
     * E.g. when the interrupt edge is "low" and the pin is still low, flag remains set.
     */
    static void clearLEDNPinInterruptFlag();

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
    static unsigned long measureLight();

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
