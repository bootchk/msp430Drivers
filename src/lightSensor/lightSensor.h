
/*
 * Abstracts light sensing.
 *
 * Implementations differ.
 * Light level that determines light/dark is parameterized in implementation.
 *
 * Binary: light or dark.
 */

class LightSensor {
public:
    static void calibrateInLightOrReset();
    static bool isDark();
    static bool isLight();
};
