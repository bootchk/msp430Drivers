Timer and Counter may both use RTC peripheral.
Timer implemented on top of Counter.
Can't use them concurrently.

Timer delays in LPM3.
LMP3 is internal.

You can use Counter separately, with external LPM3.

