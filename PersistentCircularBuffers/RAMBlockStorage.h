

#ifndef __RAMBLOCKSTORAGE_H__
#define __RAMBLOCKSTORAGE_H__


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "IBlockStorage.h"


//
// Examples of other implementations:
// - ExternalFLASHBlockStorage (over QSPI).
// - InternalFLASHBlockStorage (over internal bus)
// - ReadOnlyFLASHDevice (QSPI external with Null write methods).
//
template <uint32_t BlockSize, uint32_t numberOfBlocks> 
class RAMBlockStorage : public IBlockStorage
{

public:

    uint32_t GetBlockSize() const
    {
        return BlockSize;
    }


    void Read(void* returnedData, const uint32_t blockNumber)
    {
        memcpy( returnedData, &data[blockNumber], BlockSize );
    }

    void Write(void* dataToWrite, const uint32_t blockNumber)
    {
        memcpy( &data[blockNumber], dataToWrite, BlockSize );
    }

    void Erase(const uint32_t blockNumber)
    {
        memset( &data[blockNumber], 0xff, BlockSize );
    }


    void ReadPartial(void* returnedData, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes )
    {
        memcpy( returnedData, &data[blockNumber]+offset, numberOfBytes );
    }

    void WritePartial(void* dataToWrite, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes )
    {
        memcpy( &data[blockNumber]+offset, dataToWrite, numberOfBytes );
    }

private:
    uint8_t     data[BlockSize*numberOfBlocks];

};


#endif



