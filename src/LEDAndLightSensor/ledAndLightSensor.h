
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
     * Requires in Off state, and ensures in Off state.
     *
     * Return value indicates light level.
     * Low value means high illumination to LED
     */
    static unsigned int measureLight();

    /*
     * Understands what measurements are nighttime dark.
     */
    static bool isNighttimeDark();
};
