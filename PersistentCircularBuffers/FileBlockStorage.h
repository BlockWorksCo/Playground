

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
class FileBlockStorage : public IBlockStorage
{

public:

    uint32_t GetBlockSize() const
    {
        return BlockSize;
    }


    void Read(void* returnedData, const uint32_t blockNumber)
    {
    }

    void Write(void* dataToWrite, const uint32_t blockNumber)
    {
    }

    void Erase(const uint32_t blockNumber)
    {
    }


    void ReadPartial(void* returnedData, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes )
    {
    }

    void WritePartial(void* dataToWrite, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes )
    {
    }

private:
    int     fileHandle;

};


#endif



