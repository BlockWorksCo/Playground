


#include "WriteCoalescingBlockDevice.h"







int main()
{
    //
    // Initialise the BlockDevice.
    //
    WriteCoalescingBlockDeviceInitialise();

    //
    //
    //
    WriteCoalescingBlockDeviceWrite(0,  16, "Hello World.");
    WriteCoalescingBlockDeviceWrite(16, 16, "Hello World.");

    //
    // Flush all data to the BlockDevice.
    //
    WriteCoalescingBlockDeviceFlush();
}



