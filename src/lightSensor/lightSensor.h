
/*
 * Abstracts light sensing.
 *
 * Implementations differ.
 *
 */

class LightSensor {
public:
    static void calibrateInLightOrReset();
    static bool isDark();
};
