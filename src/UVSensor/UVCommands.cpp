
#include "UVCommands.h"

#include "UVSensorRegisters.h"

#include "../bridge/bridge.h"

#include "UVSensor.h"

// DriverLib
#include <msp430.h>     // __delay_cycles


enum Response {
        RSP0_CHIPSTAT_MASK  = 0xE0, // Chip state mask
        RSP0_COUNTER_MASK   = 0x0F, // Command counter mask
        RSP0_ERROR_MASK     = 0x10, // error indicator mask
        RSP0_SLEEP          = 0x20, // Sleep state indicator bit mask
    };





unsigned int
UVCommands::waitUntilSleep() {
  unsigned int fail;
  unsigned char response;
  unsigned int count = 0;

  /*
   * Loop until the Si1133 is in its sleep state
   *  or an i2c error occurs
   *  or count exceeded
   */
  while ( count < 5 ) {
    fail = getResponseWithErrorCheck(&response);
    if ( fail ) return fail;

    bool isSleep = (response & (unsigned char)RSP0_CHIPSTAT_MASK)
            == (unsigned char)RSP0_SLEEP;
    if ( isSleep ) {
      return 0;
    }
    count++;
  }

  // tries exceeded
  return UVSensor::Error::TimingError;
}



// !!! not for reset command count
unsigned int
UVCommands::waitForConsistentCommandCount(unsigned int * commandCount) {
    unsigned char response;
    unsigned int firstCount, secondCount;
    unsigned int count = 0;
    unsigned int fail;


    fail = getResponseWithErrorCheck(&response);
    if ( fail ) return fail;

    firstCount = response & (unsigned char)RSP0_COUNTER_MASK;

    while ( count < 5 ) {
       fail = waitUntilSleep();
       if ( fail )  return fail;

       fail = getResponseWithErrorCheck(&response);
       if ( fail) return fail;

       secondCount = response & (unsigned char)RSP0_COUNTER_MASK;
       if ( firstCount == secondCount ) {
           // Desired outcome:  we read the response count with same value twice in a row.
           *commandCount = firstCount;
           return 0;
       }
       else {
           firstCount = secondCount;
           // try again
       }
       count++;
    }

    // Failed to read consistent command count in 5 tries.
    return UVSensor::Error::TimingError;
}



unsigned int
UVCommands::waitForChangedCommandCount(unsigned int priorCommandCount) {
    unsigned int count = 0;
    unsigned char response;
    unsigned int newCount;
    unsigned int fail;

    while ( count < 5 ) {
        // Why don't we waitUntilSleep here ?

       fail = getResponseWithErrorCheck(&response);
       if ( fail) {
            return fail;
       }

       newCount = response & (unsigned char)RSP0_COUNTER_MASK;
       if ( newCount != priorCommandCount ) {
           // desired outcome
           return 0;
       }
       else {
           count++;
       }
    }
    // Failed to change count
    return UVSensor::Error::TimingError;
}




/*
 * Helper function to send a command to the Si1133
 *
 * Waits for command to finish.
 * Only required for writes to Command register.
 * !!! Except the ResetCommandCounter command.
 *
 * Returns zero on OK, non-zero otherwise
*/

unsigned int
UVCommands::sendCommand(UVCommand command)
{
    unsigned int fail;
    unsigned int priorCommandCount;

    fail = waitForConsistentCommandCount(&priorCommandCount);
    if (fail) return fail;

    /* Set command register */
    fail = setCommand(command);
    if ( fail ) return fail;

    fail = waitForChangedCommandCount(priorCommandCount);

    return fail;
}

/*
 * TODO Bridge returns true on success.
 * Should turn it into a fail enum.
 */

unsigned int
UVCommands::getID(unsigned char * ID) {
    unsigned int fail = not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::Identifier), ID);
    return fail;
}

unsigned int
UVCommands::getIRQStatus(unsigned char * status) {
    unsigned int fail = not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::IRQStatus), status);
    return fail;
}

unsigned int
UVCommands::setIRQEnable(unsigned char value) {
    unsigned int fail = not Bridge::writeByte(static_cast<unsigned char>(UVSensorAddress::IRQEnable), value);
    return fail;
}

unsigned int
UVCommands::setCommand(UVCommand command) {
    unsigned int fail = not Bridge::writeByte(static_cast<unsigned char>(UVSensorAddress::Command), command);
    return fail;
}


/*
 * Also returns error if error bit set in response
 */
unsigned int
UVCommands::getResponseWithErrorCheck(unsigned char *buffer) {
    unsigned int fail = getResponse0(buffer);
    if (fail) return fail;

    // If the error bit is set, return an error *on the prior command*
    if (*buffer & RSP0_ERROR_MASK) return UVSensor::DeviceError;
    else return UVSensor::SensorOK;
}


unsigned int
UVCommands::getResponse0(unsigned char *buffer) {
    unsigned int fail = not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::Response0), buffer);
    return fail;
}
unsigned int
UVCommands::getResponse1(unsigned char *buffer) {
    unsigned int fail = not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::Response1), buffer);
    return fail;
}


unsigned int
UVCommands::setInput(unsigned char value) {
    unsigned int fail = not Bridge::writeByte(static_cast<unsigned char>(UVSensorAddress::Input0), value);
    return fail;
}


/*
 * These are *sends*, not *set*, which means timing is handled
 */
unsigned int UVCommands::sendCommandForceRead()           { return sendCommand(UVCommand::ForceRead); }

/*
 * This is a simple *set*
 */
unsigned int UVCommands::sendCommandResetCommandCounter() { return setCommand(UVCommand::ResetCommandCounter); }
// This isn't sendCommand since caller delays anyway
unsigned int UVCommands::sendCommandReset() { return setCommand(UVCommand::Reset); }



/*
 * Read two bytes of 16-bit sample, one at a time, and form a 16-bit value
 */
unsigned int
UVCommands::getUVSample(long* sample) {
    unsigned char lowerByte, middleByte, upperByte;
    unsigned int fail;

    // Big endian: MSB is at lower index

    fail = not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::HostOut), &upperByte);
    fail += not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::HostOut1), &middleByte);
    fail += not Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::HostOut2), &lowerByte);

    // piece together 24 bit result
    unsigned long result;
    result = (unsigned long) upperByte << 16;
    result |= middleByte << 8;
    result |= lowerByte;

    if ( result & 0x800000 ) {  // if 24-bit negative
        result |= 0xFF000000;  // extend sign
    }

    *sample = result;
    return fail;
}



unsigned int
UVCommands::tryChannelZeroMeasureComplete() {
    unsigned int fail;
    unsigned char status;
    unsigned int count;

    fail =  getIRQStatus(&status);

    count = 0;
    while ( (status != 0x01) and (count < 100) and not fail ) {
        __delay_cycles(5000);   // 5 mSec
        fail =  getIRQStatus(&status);
        count++;
    }
    // assert status bit set or count>100 or fail
    return fail;
}




void
UVCommands::sleepWhileBooting() {
    __delay_cycles(30000);
}

void
UVCommands::sleepWhileResetting() {
    __delay_cycles(10000);
}
void
UVCommands::sleepWhileReadingChannel() {
    // 200 mSec for four channels, 50mSec for one channel
    __delay_cycles(200000);
}



unsigned int
UVCommands::enableIRQChannelZero() {
   unsigned int fail = setIRQEnable(0x01);
   return fail;
}

