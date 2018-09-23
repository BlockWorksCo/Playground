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

#include "MicroBit.h"
#include "UserInterfaceService.h"

MicroBit                uBit;
UserInterfaceService*   uiService;



MicroBitMessageBus bus;
MicroBitSerial serial(USBTX,USBRX);

extern void dprintf( const char* format, ... );




char UID_NAMESPACE[] = {0x4d,0xb9,0x0e,0xf2,0xf9,0xd6,0xfd,0xc9,0x71,0x7a}; // sha-1 hash of "blockworks.co" 
char UID_INSTANCE[]  = {0x00 , 0x00, 0x00, 0x00, 0x00, 0x01};
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};


// we use events abd the 'connected' variable to keep track of the status of the Bluetooth connection
void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}



//
//
//
void transmitPacket( uint8_t* packet, uint32_t numberOfBytes )
{
    uiService->send( packet, numberOfBytes );
}

uint8_t     packetQ[128];
uint32_t    numberOfBytesInQ   = 0;
uint32_t    tick                = 0;

void packetReceived( uint8_t protocolVersion, uint8_t* packet, uint32_t numberOfBytes )
{
    if( protocolVersion == 0 )
    {
        memcpy( &packetQ[0], packet, numberOfBytes );
        numberOfBytesInQ        = numberOfBytes;
    }
}

//
//    0         1        2  3  4  5  6  7
// 7e <version> <length> 00 11 22 33 44 7e
//
// 7e000700112233447e
// 7e00080011223344557e
// 7e001e00112233445566778899 aabbccddeeff00112233445566778899aabb7e
//
void byteReceived( uint8_t c )
{
    static uint8_t  packet[128]             = {0};
    static uint8_t  currentPosition         = 0;
    static bool     inPacket                = false;
    static uint32_t expectedPacketLength    = 0;
    static uint8_t  protocolVersion         = 0;
    static uint32_t lastByteReceivedTime    = 0;

    // If we have not received a byte in 1 second then timeout
    // the parser-state by resetting it.
    uint32_t    timeSinceLastByte   = tick - lastByteReceivedTime;
    if( timeSinceLastByte >= 10 )
    {
        inPacket        = false;
    }

    // Process the byte.
    if( inPacket == false )
    {
        if( c == 0x7e )
        {
            inPacket        = true;
            currentPosition = 0;
        }
    }
    else
    {
        if( (c == 0x7e) && (currentPosition == expectedPacketLength) )
        {
            packetReceived( protocolVersion, &packet[0], currentPosition-2 );
            inPacket    = false;
        }
        else
        {
            if( currentPosition == 0 )
            {
                protocolVersion         = c;
            }
            else if( currentPosition == 1 )
            {
                expectedPacketLength    = c;
            }
            //else if( (size_t)(currentPosition-2) < sizeof(packet) )
            else 
            {
                packet[currentPosition-2] = c;
            }

            currentPosition++;
        }        
    }

    // Keep track of the time of the last byte.
    lastByteReceivedTime    = tick;
}

uint8_t     data[]  = {'<', 0x02, '>'};
extern char  t[128];

void testFiber()
{
    while(true)
    {
        //static uint32_t j=0;
        //j++;
        //dprintf("Tick %d ...\r\n ",j);
        fiber_sleep(100);

        // timeout ticker.
        tick++;

        // process packet.
        if( numberOfBytesInQ > 0 )
        {
            dprintf("\r\n");
            for(uint32_t i=0; i<numberOfBytesInQ; i++ )
            {
                dprintf("%02x ",packetQ[i]);
            }
            numberOfBytesInQ    = 0;
        }

        uint8_t url[] = "https://blockworks.co/00112233";
        uiService->send( url, sizeof(url) );
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // initialise the fiber scheduler.
    scheduler_init(bus);

    uint8_t text[]    = "\r\nBlockWorks demo one.\r\n";
    serial.send( &text[0], strlen((char*)text) );


    // Services/Pairing Config/Power Level
    uBit.display.scroll("BlockWorks  ");

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    //
    //
    //
    uiService   = new UserInterfaceService(*uBit.ble);

    //
    //
    //
    const GapAdvertisingData    advertisingData;

    uBit.ble->clearAdvertisingPayload();


    uint16_t    shortUID    = 0x1234;
    //uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t*)&shortUID, sizeof(shortUID));

    uint8_t     nameData[]      = "BlockWorks1";

    uBit.ble->clearAdvertisingPayload();
    uBit.ble->setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t*)&shortUID, sizeof(shortUID));
    uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, nameData, sizeof(nameData));

    uBit.ble->setAdvertisingInterval(100);
    uBit.ble->setAdvertisingData(advertisingData);
    uBit.ble->startAdvertising();

    //
    //
    //
    create_fiber( testFiber );

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
