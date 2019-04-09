
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





class LEDAndLightSensor {

private:

    static void toReversedFromOff();

    static void toMeasuringFromReversed();

    static unsigned int measureByBleeding();

    static void toOffFromMeasuring();

    static bool isLow();

public:
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
     * Low value means high illumination to LED
     */
    static unsigned int measureLight();

    /*
     * Understands what measurements are nighttime dark.
     * WAS in relation to a constant.
     * NOW in relation to calibrated reference value.
     */
    static bool isNighttimeDark();
};
