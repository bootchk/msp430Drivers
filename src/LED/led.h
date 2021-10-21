
/*
 * Knows app's use of LED.
 *
 * Currently two LED's, same as LaunchPad.
 * Typically LED1 is  red, LED2 is green.
 *
 * For single-ended LED.
 * Understands LED may be sunk or sourced.
 */

class LED {
public:
	static void configureLED1();
	static void configureLED2();

	/*
	 * LED1
	 */
	static void turnOnLED1();
	static void turnOffLED1();
    static void toggleLED1();

    /*
     * LED2
     */
    static void turnOnLED2();
	static void turnOffLED2();
};
