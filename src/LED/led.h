
/*
 * Knows app's use of LED.
 *
 * Currently two LED's, same as LaunchPad
 */

class LED {
public:
	static void configureLED1();
	static void configureLED2();

	static void turnOn();
	static void turnOff();
	static void turnOffLED2();
	static void toggle();

	/*
	 * Busy wait blink.
	 */
	static void blink();
};
