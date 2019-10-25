
i2c is the top level

i2cPeripheral is direct access to the peripheral, where DriverLib is insufficient

i2cTransport is the transport layer, calls link layer and i2cPeripheral

Several link layers implementations:

    i2cDirect:  a hack I used for testing.  Manipulates peripheral registers.
    
    stateMachine:  an interrupt driven approach.  Not working, halts in initialTransition() sometimes???
    
    i2cDriverLibLink:  uses TI DriverLib