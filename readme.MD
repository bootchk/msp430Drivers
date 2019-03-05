Work in progress

High level drivers.
Abstracts peripherals on TI MSP430.


Pragmatics
==========

Currently not built as a linkable library.
Copy src into a folder into your project (or link a folder in your project to msp430Drivers/src.)
And add an include path to that folder.


Requires TI MSPWare DriverLib
=============================

You might be able to use TI Resource Explorer.
On MacOS, download standalone MSPWare installer and install to /applications/ti.
Then the TI Resource Explorer will find it when you restart CCS.

Or download the BSD licensed source anywhere (say to your download folder.)

Once installed, create a linked folder in your project to the installed location of MSPWare,
specifically the version for your processor family,  say "MSP430FR2xx_4xx".
That makes the DriverLib source visible in the project and Eclipse will compile it.
But you also need to add an include path (in the project properties) to the downloaded location.
(A include path to the linked folder doesn't seem to work, you need an include path to the DriverLib directory.)


Configuration
=============

board.h defines the configuration of your PCB (the functions of GPIO pins of the mcu.)

config.h defines other configuration of the library (for testing versus production.)

driverParameters defines constants for certain drivers


Layers
======

Loosely speaking, there are high level drivers and low-level drivers.
The high-level drivers should be platform independent.
The low-level drivers target a specific platform family.
But a platform specific library of lower-level drivers (e.g. DriverLib for the MSP430) are independent of specific family member.
