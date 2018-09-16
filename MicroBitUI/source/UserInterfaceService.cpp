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

/**
  * Class definition for the custom MicroBit UART Service.
  * Provides a BLE service that acts as a UART port, enabling the reception and transmission
  * of an arbitrary number of bytes.
  */

#include "ble/UUID.h"
#include "MicroBit.h"
#include "ExternalEvents.h"
#include "UserInterfaceService.h"
#include "MicroBitFiber.h"
#include "ErrorNo.h"
#include "NotifyEvents.h"
#include <stdarg.h>

extern MicroBitSerial serial;

static uint8_t txBufferHead = 0;
static uint8_t txBufferTail = 0;

static GattCharacteristic* txCharacteristic = NULL;



const uint8_t  UIServiceBaseUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x6E, 0x40, 0x00, 0x00, 0xB5, 0xA3, 0xF3, 0x93,
    0xE0, 0xA9, 0xE5, 0x0E, 0x24, 0xDC, 0xCA, 0x9E,
};
const uint16_t UIServiceShortUUID                 = 0x0001;
const uint16_t UIServiceTXCharacteristicShortUUID = 0x0002;
const uint16_t UIServiceRXCharacteristicShortUUID = 0x0003;
const uint8_t  UIServiceUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x6E, 0x40, (uint8_t)(UIServiceShortUUID >> 8), (uint8_t)(UIServiceShortUUID & 0xFF), 0xB5, 0xA3, 0xF3, 0x93,
    0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
};
const uint8_t  UIServiceTXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x6E, 0x40, (uint8_t)(UIServiceTXCharacteristicShortUUID >> 8), (uint8_t)(UIServiceTXCharacteristicShortUUID & 0xFF), 0xB5, 0xA3, 0xF3, 0x93,
    0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
};
const uint8_t  UIServiceRXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x6E, 0x40, (uint8_t)(UIServiceRXCharacteristicShortUUID >> 8), (uint8_t)(UIServiceRXCharacteristicShortUUID & 0xFF), 0xB5, 0xA3, 0xF3, 0x93,
    0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
};


//
//
//
void dprintf( const char* format, ... )
{
    va_list         argList;
    static char     t[128]   = {0};

    va_start( argList, format );

    vsprintf( t, format, argList );
    serial.send( (uint8_t*)&t[0], strlen(t) );

    va_end(argList);
}



/**
  * A callback function for whenever a Bluetooth device consumes our TX Buffer
  */
void on_confirmation(uint16_t handle)
{
    dprintf("on confirm\n");

    if(handle == txCharacteristic->getValueAttribute().getHandle())
    {
        txBufferTail = txBufferHead;
    }
}

/**
 * Constructor for the UIService.
 * @param _ble an instance of BLEDevice
 * @param rxBufferSize the size of the rxBuffer
 * @param txBufferSize the size of the txBuffer
 *
 * @note defaults to 20
 */
UserInterfaceService::UserInterfaceService(BLEDevice &_ble) : ble(_ble)
{
    rxBufferSize = 32;
    txBufferSize = 32;

    txBuffer = (uint8_t *)malloc(txBufferSize);
    rxBuffer = (uint8_t *)malloc(rxBufferSize);

    rxBufferHead = 0;
    rxBufferTail = 0;

    txBufferHead = 0;
    txBufferTail = 0;

    //
    //
    //
    GattCharacteristic  rxCharacteristic(UIServiceRXCharacteristicUUID, rxBuffer, 0, rxBufferSize,  GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
                                                                                                    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
    GattCharacteristic  txCharacteristic(UIServiceTXCharacteristicUUID, txBuffer, 0, txBufferSize,  GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE | 
                                                                                                    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | 
                                                                                                    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE );
    static GattCharacteristic *charTable[] = {&txCharacteristic, &rxCharacteristic};

    static GattService uartService(UIServiceUUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

    _ble.addService(uartService);

    txCharacteristicHandle = txCharacteristic.getValueAttribute().getHandle();
    rxCharacteristicHandle = rxCharacteristic.getValueAttribute().getHandle();

    _ble.gattServer().onDataWritten( this, &UserInterfaceService::onDataWritten );
    _ble.gattServer().onConfirmationReceived( on_confirmation );
    _ble.gattServer().onDataRead( this, &UserInterfaceService::onDataRead );

    dprintf("Started up service...\n");
    bprintf("Started up service...\n");

}


//
//
//
void UserInterfaceService::bprintf( const char* format, ... )
{
    va_list         argList;
    static char     t[128]   = {0};

    va_start( argList, format );

    vsprintf( t, format, argList );
    ble.gattServer().write( rxCharacteristicHandle, (uint8_t*)&t[0], strlen(t) );

    va_end(argList);
}


//
//
//
void UserInterfaceService::onDataRead(const GattReadCallbackParams* params) 
{
    dprintf("onDataRead\n");
}



/**
  * A callback function for whenever a Bluetooth device writes to our RX characteristic.
  */
void UserInterfaceService::onDataWritten(const GattWriteCallbackParams *params) 
{
    if (params->handle == txCharacteristicHandle)
    {
        uint16_t bytesWritten = params->len;

        static char     t[64]   = {0};
#if 0
        sprintf( &t[0], "(%d)", bytesWritten );
#else
        for(uint16_t i = 0; i <  bytesWritten; i++)
        {
            char c = params->data[i];
            sprintf( &t[i*3], "%02x ", c );
        }
#endif
        serial.send( (uint8_t*)&t[0], bytesWritten*3 );
    }
}

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
void UserInterfaceService::circularCopy(uint8_t *circularBuff, uint8_t circularBuffSize, uint8_t *linearBuff, uint16_t tailPosition, uint16_t headPosition)
{
    int toBuffIndex = 0;

    while(tailPosition != headPosition)
    {
        linearBuff[toBuffIndex++] = circularBuff[tailPosition];

        tailPosition = (tailPosition + 1) % circularBuffSize;
    }
}


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
int UserInterfaceService::send(const uint8_t *buf, int length, MicroBitSerialMode mode)
{
    static int c=0;
    bprintf("<%d>",c++);

    if(length < 1 || mode == SYNC_SPINWAIT)
        return MICROBIT_INVALID_PARAMETER;

    bool updatesEnabled = false;

    ble.gattServer().areUpdatesEnabled(*txCharacteristic, &updatesEnabled);

    if(!ble.getGapState().connected && !updatesEnabled)
        return MICROBIT_NOT_SUPPORTED;

    int bytesWritten = 0;

    while(bytesWritten < length && ble.getGapState().connected && updatesEnabled)
    {
        for(int bufferIterator = bytesWritten; bufferIterator < length; bufferIterator++)
        {
            int nextHead = (txBufferHead + 1) % txBufferSize;

            if(nextHead != txBufferTail)
            {
                txBuffer[txBufferHead] = buf[bufferIterator];

                txBufferHead = nextHead;

                bytesWritten++;
            }
        }

        int size = txBufferedSize();

        uint8_t temp[size];

        memclr(&temp, size);

        circularCopy(txBuffer, txBufferSize, temp, txBufferTail, txBufferHead);


        if(mode == SYNC_SLEEP)
            fiber_wake_on_event(MICROBIT_ID_NOTIFY, MICROBIT_UART_S_EVT_TX_EMPTY);

        ble.gattServer().write(txCharacteristic->getValueAttribute().getHandle(), temp, size);

        if(mode == SYNC_SLEEP)
            schedule();
        else
            break;

        ble.gattServer().areUpdatesEnabled(*txCharacteristic, &updatesEnabled);
    }

    return bytesWritten;
}


/**
  * @return The currently buffered number of bytes in our txBuff.
  */
int UserInterfaceService::txBufferedSize()
{
    dprintf("txBufferedSize\r\n");

    if(txBufferTail > txBufferHead)
        return (txBufferSize - txBufferTail) + txBufferHead;

    return txBufferHead - txBufferTail;
}


