
/*
 * Interface to I2C peripheral of MSP430
 *
 *
 */


// TODO Move functions here from I2CTransport




class I2CPeripheral {
public:



    /*
     * Interrupt flags ( register EUSCIB0IFG )
     */

    static void clearInterruptFlags();

    /*
     * Are all interrupt flags clear?
     * !!! Not true after reset, since some flags (e.g. UCTXIFG3) are on after reset.
     */
     static bool isNoInterruptFlag();

     static unsigned int getInterruptFlags();





     /*
      * Busy means: start condition detected (SDA going low while SCL is high.)
      * If there is no other master, you can ignore this?
      */
     static bool isBusBusy();

     /*
      * Spin until not isBusBusy().
      * May send stop if bus is busy
      * Other side effects.
      */
     static void waitUntilBusReady();

     /*
      * Wait until prior stop has been transmitted.
      * I.E. until prior transport is done.
      */
     static void waitUntilPriorTransportComplete();

};
