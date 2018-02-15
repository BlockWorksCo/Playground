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





char UID_NAMESPACE[] = {0x0E,0x67,0x47,0x04,0x42,0xD0,0x14,0x06,0xD5,0x83}; // sha-1 hash of "com.bittysoftware" 
char UID_INSTANCE[]  = {0x00 , 0x00, 0x00, 0x00, 0x00, 0x01};
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};

uint8_t advertising = 0;
uint8_t tx_power_level = 6;

/*
void startAdvertising() {
    uBit.bleManager.advertiseEddystoneUid(UID_NAMESPACE, UID_INSTANCE, CALIBRATED_POWERS[tx_power_level-1], false);
    uBit.bleManager.setTransmitPower(6);
    uBit.display.scroll("ADV");
    advertising = 1;
}

void stopAdvertising() {
    uBit.bleManager.stopAdvertising();
    uBit.display.scroll("OFF");
    advertising = 0;
}
*/


// we use events abd the 'connected' variable to keep track of the status of the Bluetooth connection
void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}

uint8_t     data[]  = {'<', 0x02, '>'};

void testFiber()
{
    while(true)
    {
        uBit.display.print("X");
        fiber_sleep(1000);

        uBit.display.print("O");
        fiber_sleep(1000);

        uiService->send( data, sizeof(data) );
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Configuration Tips
    //
    // config.json contains various Bluetooth related properties some of which are explained here:
    //
    // "dfu_service": 1,               // 1 will cause the DFU service to be instantiated
    // "event_service": 1,             // 1 causes the event service to be instantiated
    // "device_info_service": 1
    // "enabled": 1,                   // 1 means the Bluetooth stack will be included as standard. 0 means it will not.
    // "pairing_mode": 1,              // 1 means it's possible to go into pairing mode which will include bringing up the Bluetooth stack whilst in that mode.
    // "open": 0,                      // 1 means there's no Bluetooth security i.e. no need to pair the micro:bit with other devices that want to communicate with it.
    // "tx_power": 7,                  // Transmission power of the Bluetooth radio. A value of 0 - 7 with 0 the lowest power and 7 the highest power.
    // "gatt_table_size": "0x700"      // Amount of memory (in hex bytes) set aside for the Bluetooth GATT table
    // "nested_heap_proportion": 0.75, // Reducing this can sometimes help make enough memory available for all the Bluetooth services you want. Only experiment with this as a last resort.

    // MicrobitConfig.h in yotta_modules\microbit-dal\inc\core contains MICROBIT_BLE_SECURITY_LEVEL which can be set to SECURITY_MODE_ENCRYPTION_WITH_MITM for passkey authentication when
    // pairing or SECURITY_MODE_ENCRYPTION_NO_MITM to use Just Works pairing.

    // A cunning code to indicate during start-up the particular Bluetooth configuration in the build
    //
    // SERVICE CODES
    // A: Accelerometer Service
    // B: Button Service
    // D: Device Information Service
    // E: Event Service
    // F: DFU Service
    // I: IO Pin Service
    // L: LED Service
    // M: Magnetometer Service
    // T: Temperature Service
    // U: UART Service
    //
    // PAIRING CONFIG
    // Note that switching pairing on or off is achieved by setting "open" in config.json to 1 or 0 respectively

    // P: PASSKEY
    // J: Just Works
    // N: No Pairing Required
    //
    // TX Power Level
    // 0-7 taken from tx_power in config.json


    // Services/Pairing Config/Power Level
    uBit.display.scroll("BLE U/N/0");

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    //new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    //new MicroBitButtonService(*uBit.ble);
    //new MicroBitIOPinService(*uBit.ble, uBit.io);
    //new MicroBitLEDService(*uBit.ble, uBit.display);
    //new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    //new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    uiService   = new UserInterfaceService(*uBit.ble);

    //
    //
    //
    const GapAdvertisingData    advertisingData;
    uBit.ble->setAdvertisingData(advertisingData);

    //uBit.ble->clearAdvertisingPayload();
    //uBit.ble->setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    uBit.ble->setAdvertisingInterval(100);
    //uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    //uint16_t    shortUID    = 0x1234;
    //uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t*)&shortUID, sizeof(shortUID));

    //uint8_t serviceData[]   = {0x00,0x012};
    //uint32_t    serviceDataLen  = sizeof(serviceData);
    //uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, serviceData, serviceDataLen);

    uBit.bleManager.advertiseEddystoneUid(UID_NAMESPACE, UID_INSTANCE, CALIBRATED_POWERS[tx_power_level-1], false);
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
