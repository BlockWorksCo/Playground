/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef MICROBIT_UART_SERVICE_H
#define MICROBIT_UART_SERVICE_H

#include "mbed.h"
#include "ble/UUID.h"
#include "ble/BLE.h"


/**
  * Class definition for the custom MicroBit UART Service.
  * Provides a BLE service that acts as a UART port, enabling the reception and transmission
  * of an arbitrary number of bytes.
  */
class UserInterfaceService
{
    uint8_t* rxBuffer;

    uint8_t* txBuffer;

    uint8_t rxBufferHead;
    uint8_t rxBufferTail;
    uint8_t rxBufferSize;

    uint8_t txBufferSize;

    uint32_t txCharacteristicHandle;
    uint32_t rxCharacteristicHandle;

    // Bluetooth stack we're running on.
    BLEDevice           &ble;

    //
    void bprintf( const char* format, ... );

    /**
      * A callback function for whenever a Bluetooth device writes to our TX characteristic.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

    //
    //
    //
    void onDataRead(const GattReadCallbackParams* params);
 
    /**
      * An internal method that copies values from a circular buffer to a linear buffer.
      *
      * @param circularBuff a pointer to the source circular buffer
      * @param circularBuffSize the size of the circular buffer
      * @param linearBuff a pointer to the destination linear buffer
      * @param tailPosition the tail position in the circular buffer you want to copy from
      * @param headPosition the head position in the circular buffer you want to copy to
      *
      * @note this method assumes that the linear buffer has the appropriate amount of
      *       memory to contain the copy operation
      */
    void circularCopy(uint8_t *circularBuff, uint8_t circularBuffSize, uint8_t *linearBuff, uint16_t tailPosition, uint16_t headPosition);

    public:

    /**
     * Constructor for the UARTService.
     * @param _ble an instance of BLEDevice
     * @param rxBufferSize the size of the rxBuffer
     * @param txBufferSize the size of the txBuffer
     *
     * @note The default size is MICROBIT_UART_S_DEFAULT_BUF_SIZE (20 bytes).
     */
    UserInterfaceService(BLEDevice &_ble);

    /**
      * Copies characters into the buffer used for Transmitting to the central device.
      *
      * @param buf a buffer containing length number of bytes.
      * @param length the size of the buffer.
      * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
      *        gives a different behaviour:
      *
      *            ASYNC - Will copy as many characters as it can into the buffer for transmission,
      *                    and return control to the user.
      *
      *            SYNC_SPINWAIT - will return MICROBIT_INVALID_PARAMETER
      *
      *            SYNC_SLEEP - Will perform a cooperative blocking wait until all
      *                         given characters have been received by the connected
      *                         device.
      *
      * @return the number of characters written, or MICROBIT_NOT_SUPPORTED if there is
      *         no connected device, or the connected device has not enabled indications.
      */
    int send(const uint8_t *buf, int length, MicroBitSerialMode mode = SYNC_SLEEP);

    /**
      * Copies characters into the buffer used for Transmitting to the central device.
      *
      * @param s the string to transmit
      * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
      *        gives a different behaviour:
      *
      *            ASYNC - Will copy as many characters as it can into the buffer for transmission,
      *                    and return control to the user.
      *
      *            SYNC_SPINWAIT - will return MICROBIT_INVALID_PARAMETER
      *
      *            SYNC_SLEEP - Will perform a cooperative blocking wait until all
      *                         given characters have been received by the connected
      *                         device.
      *
      * @return the number of characters written, or MICROBIT_NOT_SUPPORTED if there is
      *         no connected device, or the connected device has not enabled indications.
      */
    int send(ManagedString s, MicroBitSerialMode mode = SYNC_SLEEP);

    /**
      * Determines if we have space in our rxBuff.
      *
      * @return 1 if we have space, 0 if we do not.
      */
    int isReadable();

    /**
      * @return The currently buffered number of bytes in our rxBuff.
      */
    int rxBufferedSize();

    /**
      * @return The currently buffered number of bytes in our txBuff.
      */
    int txBufferedSize();
};

extern const uint8_t  UARTServiceBaseUUID[UUID::LENGTH_OF_LONG_UUID];
extern const uint16_t UARTServiceShortUUID;
extern const uint16_t UARTServiceTXCharacteristicShortUUID;
extern const uint16_t UARTServiceRXCharacteristicShortUUID;

extern const uint8_t  UARTServiceUUID[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t  UARTServiceUUID_reversed[UUID::LENGTH_OF_LONG_UUID];

extern const uint8_t  UARTServiceTXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t  UARTServiceRXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID];

#endif
