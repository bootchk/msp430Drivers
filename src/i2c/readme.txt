
i2c is the top level

i2cPeripheral is direct access to the peripheral, where DriverLib is insufficient

i2cTransport is the transport layer, calls link layer and i2cPeripheral

Several link layers implementations:

    i2cDirect:  a hack I used for testing.  Manipulates peripheral registers.
    
    stateMachine:  an interrupt driven approach.  Not working, halts in initialTransition() sometimes???
    
    i2cDriverLibLink:  uses TI DriverLib, but designed myself
    
    i2cDriverLibFromIT:  uses TI DriverLib, but following driverlib/examples/ ...FR2xx.../eusci_b_i2c/...masterRxMultiple.c etc.
    

I thrashed with multiple implementations.  Only to find it was probably a bug outside this library that caused I2C errors (SMCLK stopped inadvertantly.)
Nov. 2019 I left it with most implementations in an untested state, and myself only using i2cDriverLibFromIT / i2cDriverLibLinkWISR.
Probably stateMachine would work, it seemed to work until said bug outside this library caused it to stop working.