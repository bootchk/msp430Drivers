/*
 * A manager of a group of GPIO pins
 *
 * Specific to a particular MPS430 model.
 */

class GpioGroup {
public:

    // Class methods (static)

    /*
     * Configure all gpio pins to have low values (when configured as outputs.)
     */
    static void setAllOutputsLow();

    /*
     * Configure all gpio pins to be outputs.
     * (So they will use low power when sleeping.)
     */
    // configure all GPIO out to ensure low power
   static void configureGPIOLowPower();
};
