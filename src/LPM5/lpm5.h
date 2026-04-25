class LPM5{

public:
	static void haltWithRTCWakeup();

	static void configureRTCToWakeup();
	static void unlockConfiguration();

	static void enableWakeupInterrupt();
	static void disableWakeupInterrupt();
};
