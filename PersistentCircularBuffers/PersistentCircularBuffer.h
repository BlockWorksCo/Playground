


#ifndef __PERSISTENTCIRCULARBUFFER_H__
#define __PERSISTENTCIRCULARBUFFER_H__


#include <stdint.h>
#include <stdbool.h>
#include <string.h>



//
// PersistentCircularBuffer stores a number of elements in a specified BlockStorage. The type of 
// the elements is also specified.
//
// Key points here:
// - Naturally wear-levelling, with deterministic over-allocation.
// - Elements are integrity checked.
// - Integrity checking leads to atomic-write property.
// - Integrity checking leads to easy bad-block management (skip the block).
// - Pre-erasing means quick, deterministic writing of data in shutdown case.
// - Easy extension to add encryption.
// - Sanitisation is just a flush&clear of the circular buffer.
// - Caching of latest block gives optimal performance, power usage and minimal-erase-cycles (read on startup, write on shutdown).
//
template <typename BlockStorageType, typename ContainedType, uint32_t NumberOfElements>
class PersistentCircularBuffer
{

public:

    //
    // Initialise the member-reference to the BlockStorage. This allows the optimiser to directly
    // reference the BlockStorage instead of using an indirection (pointer), thus making it safer
    // and more efficient.
    //
    PersistentCircularBuffer( BlockStorageType& _BlockStorage ) : 
        BlockStorage(_BlockStorage),
        firstElement(0),
        lastElement(0),
        cursorPosition(0)
    {
        //
        // - Perform search for last-good-position if not supplied from checkpoint data.
        // - Perform a integrity check of the cache. If we have good contents, use it.
        //
    }


    //
    // Cursor movement (for log access/searching).
    //
    void MoveToLast()
    {
        cursorPosition  = lastElement;
    }

    void MoveToFirst()
    {
        cursorPosition  = firstElement;
    }

    void Next()
    {
        cursorPosition     = (cursorPosition + 1) % NumberOfElements;
    }

    void Last()
    {
        cursorPosition     = (cursorPosition - 1) % NumberOfElements;
    }

    //
    // Methods for adding & removing elements.
    //
    void Append()
    {
        lastElement     = (lastElement + 1) % NumberOfElements;
        MoveToLast();
    }

    void RemoveFirst()
    {
        firstElement    = (firstElement + 1) % NumberOfElements;
    }


    //
    // Methods to access the entire ContainedType.
    //
    void Read( ContainedType& data )
    {
        CheckIntegrityCheckValue( BlockNumberFromElementNumber(cursorPosition) );
        memcpy( &data, &CurrentCachedElement().data, sizeof(data) );
    }

    void Write( ContainedType& data )
    {
        UpdateIntegrityCheckValue( BlockNumberFromElementNumber(cursorPosition) );
        memcpy( &CurrentCachedElement().data, &data, sizeof(data) );
    }


    //
    // Methods to access the data in small chunks.
    //
    void ReadPartial( void* data, const size_t offset, const size_t dataLength )
    {
        LoadCache( BlockNumberFromElementNumber(cursorPosition) );
        CheckIntegrityCheckValue( BlockNumberFromElementNumber(cursorPosition) );
        memcpy( data, ((uint8_t*)&CurrentCachedElement().data)+offset, dataLength );
    }

    void WritePartial( void* data, const size_t offset, const size_t dataLength )
    {
        LoadCache( BlockNumberFromElementNumber(cursorPosition) );
        memcpy( ((uint8_t*)&CurrentCachedElement().data)+offset, data, dataLength );
        UpdateIntegrityCheckValue( BlockNumberFromElementNumber(cursorPosition) );
    }


    //
    // Methods to access the data in a more type-safe manner.
    //
    template <typename FieldType> void ReadField( FieldType& data, const size_t offset )
    {
        ReadPartial( &data, offset, sizeof(data) );
    }

    template <typename FieldType> void WriteField( FieldType& data, const size_t offset )
    {
        WritePartial( &data, offset, sizeof(data) );
    }


    //
    // Signify that a set of writes has completed (used for double-buffer management and reset-robustness)
    //
    void WriteSetComplete()
    {
        lastGoodCache    = (lastGoodCache+1) & 0x01;  // simply flip-flop between 0 and 1.
    }

private:

    //
    // The Element struct adds a bit of metadata to the ContainedType.
    //
    typedef struct
    {
        uint32_t        sequenceNumber;         // always incrementing and identifies latest data. This value gets stored in checkpoint CB.
        uint32_t        integrityCheckValue;    // CRC32/SHA of the element contents. Used for bad-block management and reset cases.

        ContainedType   data;

    } Element;


    //
    // Obtain the raw block number that a given element is stored in.
    //
    uint32_t BlockNumberFromElementNumber( const uint32_t elementNumber )
    {
        const uint32_t elementsPerBlock  = BlockStorage.GetBlockSize() / sizeof(Element); 
        return elementNumber / elementsPerBlock;
    }

    //
    // Calculate and update the integrity check value (CRC32/SHA) after a modification.
    //
    void UpdateIntegrityCheckValue( uint32_t blockNumber )
    {

    }

    void CheckIntegrityCheckValue( uint32_t blockNumber )
    {

    }

    //
    // Cache management.
    //
    Element& CurrentCachedElement()
    {
        return cache[(lastGoodCache+1)&0x01];
    }

    Element& LastGoodCachedElement() const
    {
        return cache[lastGoodCache];
    }

    void LoadCache( uint32_t elementNumber )
    {
        if( elementNumber != cachedElement )
        {
            FlushCache();
            BlockStorage.Read( &CurrentCachedElement(), BlockNumberFromElementNumber(cachedElement) );
            cachedElement   = elementNumber;
        }
    }

    void FlushCache()
    {
        BlockStorage.Write( &CurrentCachedElement(), BlockNumberFromElementNumber(cachedElement) );
    }

    BlockStorageType&   BlockStorage;       // reference to the raw storage.
    uint32_t            lastElement;        // index of the last-written element in the buffer.
    uint32_t            firstElement;       // index of the earliest-written element in the buffer.
    uint32_t            cursorPosition;     // Current position for read/write.
    
    //
    // These elements form the cache. For reset-robustness, they must be placed into a non-initialised region.
    // This data gets flushed to FLASH on shutdown.
    //
    Element             cache[2];           // Cache of latest element. This is an array of 2 in order to provide reset-robustness (double buffer).
    uint32_t            cachedElement;      // Index of cached element.
    uint32_t            lastGoodCache;      // index into 'cache' of last written element.
};


#endif


