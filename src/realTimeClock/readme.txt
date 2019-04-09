Code for an external Abracon RTC.

Not for the internal "RTC" which is just a counter, and not a "real time clock".


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
        |           EpochClock
        |               |
    ___________________________________
    RTC
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
       etc/