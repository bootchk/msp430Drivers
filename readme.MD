Work in progress

Requires TI MSPWare DriveLib

You might be able to use TI Resource Explorer.
On MacOS, download standalone MSPWare installer and install to /applications/ti.
Then the TI Resource Explorer will find it when you restart CCS.

Or download the BSD licensed source anywhere (say to your download folder.)

Once installed, create a linked folder "driverlib" in your project to the installed location of MSPWare,
specifically the version for your processor family e.g. MSP430FR2xx4xx

The above fails?  So just copy the driverlib directory into the project
and add an include path to the project properties.