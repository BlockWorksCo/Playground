

#ifndef __IBLOCKSTORAGE_H__
#define __IBLOCKSTORAGE_H__


#include <stdint.h>
#include <stdbool.h>


//
// A BlockStorage interface provides methods for accessing raw storage (BLOB-style)
//
class IBlockStorage
{

public:

    uint32_t GetBlockSize() const;


    void Read(void* returnedData, const uint32_t blockNumber);

    void Write(void* dataToWrite, const uint32_t blockNumber);

    void Erase(const uint32_t blockNumber);


    void ReadPartial(void* returnedData, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes );

    void WritePartial(void* dataToWrite, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes );

};


#endif



