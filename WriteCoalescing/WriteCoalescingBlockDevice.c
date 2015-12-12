


#include "WriteCoalescingBlockDevice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define NUMBER_OF_ELEMENTS(a)       (sizeof(a)/sizeof(a[0]))


//
//
//
typedef struct
{
    uint8_t     data[128];
    uint32_t    offset;
    uint32_t    numberOfBytes;

} WriteOperation;


//
//
//
typedef struct
{
    WriteOperation  operations[16];
    uint32_t        blockNumber;

} BlockOperations;


//
//
//
BlockOperations     blockOperations[16];



//
//
//
uint32_t BlockNumberOfOffset(uint32_t offset)
{
    return offset / 1024;
}


//
//
//
uint32_t OffsetForBlockNumber(uint32_t blockNumber)
{
    return blockNumber * 1024;
}



//
//
//
BlockOperations* BlockOperationsForOffset( uint32_t offset )
{
    uint32_t                thisBlockNumber     = BlockNumberOfOffset(offset);
    BlockOperations*        returnValue         = NULL;
    BlockOperations*        emptySlot           = NULL;

    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(blockOperations); i++)
    {
        if(blockOperations[i].blockNumber == thisBlockNumber)
        {
            returnValue     = &blockOperations[i];
            break;
        }

        if(blockOperations[i].blockNumber == -1)
        {
            emptySlot   = &blockOperations[i];
        }
    }

    if(returnValue == NULL)
    {
        returnValue     = emptySlot;
    }

    if(returnValue == NULL)
    {
        //PANIC();
        exit(-1);
    }

    returnValue->blockNumber    = thisBlockNumber;

    return returnValue;
}


//
//
//
void WriteCoalescingBlockDeviceInitialise()
{
    BlockDeviceInitialise();

    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(blockOperations); i++)
    {
        blockOperations[i].blockNumber  = -1;
    }
}


//
//
//
void WriteCoalescingBlockDeviceRead(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    BlockDeviceRead( offset, numberOfBytes, data );
}


//
//
//
void WriteCoalescingBlockDeviceWrite(uint32_t offset, uint32_t numberOfBytes, uint8_t* data)
{
    BlockOperations*    blockOps            = BlockOperationsForOffset( offset );
    WriteOperation*     writeOperations     = NULL;

    //
    // TODO: Combine adjacent operations.
    // TODO: Combine into any encompassing operations.
    // TODO: Remove any overwrtten operations.
    //

    //
    //
    //
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(blockOps->operations); i++)
    {
        if(blockOps->operations[i].numberOfBytes == 0)
        {
            writeOperations     = &blockOps->operations[i];
            break;
        }
    }

    if(writeOperations != NULL)
    {
        writeOperations->offset         = offset;
        writeOperations->numberOfBytes  = numberOfBytes;
        memcpy(writeOperations->data, data, numberOfBytes);
        printf("<coalesced write of %d bytes to %08x>\n", offset, numberOfBytes);
    }
    else
    {
        exit(-1);
    }

}




//
//
//
void WriteCoalescingBlockDeviceFlush()
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(blockOperations); i++)
    {
        if(blockOperations[i].blockNumber != -1)
        {
            for(uint32_t j=0; j<NUMBER_OF_ELEMENTS(blockOperations[i].operations); j++)
            {
                if(blockOperations[i].operations[j].numberOfBytes != 0)
                {
                    uint32_t    offset          = blockOperations[i].operations[j].offset;
                    uint32_t    numberOfBytes   = blockOperations[i].operations[j].numberOfBytes;
                    uint8_t*    data            = &blockOperations[i].operations[j].data[0];
                    BlockDeviceWrite( offset, numberOfBytes, data );
                    printf("<performed write of %d bytes to %08x>\n", offset, numberOfBytes);
                }
            }            
        }
    }

}



