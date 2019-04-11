Code for clock and alarm functions implemented using external Abracon RTC.

App uses only types EpochTime and Duration
Lower levels use RTCTime implemented in external chip. 

Not for the MSP430 internal "RTC" which is just a counter, and not a "real time clock".


This diagram is work in progress.
FUTURE fix it.
It is twisted around.
App should call a high level.
Separate object that understands RTC.
That calls low level RTC.
RTCTime should not be visible to top.


Architecture:

    App
    __________________________
        |               |
        |           ___________________________
     Alarm            EpochClock
        |               |
    ___________________________________
    RealTimeClock (RTC)
    ___________________________________     ^
        |               |                   |
        |           __________________________
        |              RTCInterface
        |               |
    ______________________
      Bridge
        |
     Serial
        |
       	I2C etc
       	
       	
       	
       	
       	