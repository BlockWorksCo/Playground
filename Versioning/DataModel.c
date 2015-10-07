

#include "DataModel.h"
#include "DataModelMap.h"
#include "BlockDevice.h"
#include "ErrorHandler.h"
#include "DataModelUpgrade.h"




//
//
//
static DataModelMap     map;



void ReadFromBlockDevice()
{
    BlockDeviceRead(0, sizeof(DataModelMap), (uint8_t*)&map );
}

void WriteToBlockDevice()
{
    BlockDeviceWrite(0, sizeof(DataModelMap), (uint8_t*)&map );
}






//
// Perform a cold-start (i.e. all defaults).
//
void DataModelColdStart()
{
    //
    // Force an old image into the BlockDevice.
    //
    DataModelMap    map  = 
    {
        .version    = 0x01,
        .one        = 1,
        .two        = 2,
        .three      = 3,
        .four       = 4,
    };

    BlockDeviceWrite(0, sizeof(DataModelMap), (uint8_t*)&map );

}



void DataModelInitialise()
{
    //
    // Check the version and potentially upgrade the DataModel.
    //
    ReadFromBlockDevice();

    if( map.version == 0x00 )
    {
        //
        // Upgrade the DataModel from v0.
        //
        DataModelUpgrade();
        printf("<DataModel upgraded from v0 to v1>\n");
    }
    else if( map.version == 0xff )
    {
        //
        // 0xff indicates a cold-start (erased FLASH).
        //
        DataModelColdStart();
        printf("<DataModel cold-start to v1>\n");
    }
    else if( map.version == 0x01 )
    {
        //
        // Current version (v1), leave as is.
        //
        printf("<DataModel at v1>\n");
    }
    else
    {
        //
        // Unknown version, neither old nor new so we're stuck.
        //
        printf("(v%d)\n",map.version);
        PANIC("Unknown DataModel version.");
    }
}





void DataModelGetItemOne(uint32_t* data)
{
    ReadFromBlockDevice();
    *data   = map.one;
}

void DataModelGetItemTwo(uint32_t* data)
{
    ReadFromBlockDevice();
    *data   = map.two;
}

void DataModelGetItemThree(uint32_t* data)
{
    ReadFromBlockDevice();
    *data   = map.three;
}

void DataModelGetItemFour(uint32_t* data)
{
    ReadFromBlockDevice();
    *data   = map.four;
}




void DataModelSetItemOne(uint32_t* data)
{
    map.one     = *data;
    WriteToBlockDevice();
}

void DataModelSetItemTwo(uint32_t* data)
{
    map.two     = *data;
    WriteToBlockDevice();
}

void DataModelSetItemThree(uint32_t* data)
{
    map.three   = *data;
    WriteToBlockDevice();
}

void DataModelSetItemFour(uint32_t* data)
{
    map.four    = *data;
    WriteToBlockDevice();
}



