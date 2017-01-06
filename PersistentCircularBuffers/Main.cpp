



#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <cstddef>


//
// What is here?
// - RAMBlockDevice as an example implementation of a raw storage device.
// - PersistentCircularBuffer template class that takes a BlockDevice type as aa parameter.
// - Configuration of types.
// - static Instantiation of types.
// - DataModel template class.
// - 'Main' showing different data access mechanisms.
//


//
// Examples of other implementations:
// - ExternalFLASHBlockDevice (over QSPI).
// - InternalFLASHBlockDevice (over internal bus)
// - CachedFLASHBlockDevice (QSPI with internal SRAM cache)
// - WatchdogSafeRAMBlockDevice (internal SRAM placed into non-initialised section so not initialised by pre-main startup code)
// - ReadOnlyFLASHDevice (QSPI external with Null write methods).
// - EncryptedBlockDevice.
//
template <uint32_t BlockSize, uint32_t numberOfBlocks> 
class RAMBlockDevice
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

    void ReadPartial(void* returnedData, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes )
    {
        memcpy( returnedData, &data[blockNumber]+offset, numberOfBytes );
    }

    void WritePartial(void* dataToWrite, const uint32_t blockNumber, const uint32_t offset, const uint32_t numberOfBytes )
    {
        memcpy( &data[blockNumber]+offset, dataToWrite, numberOfBytes );
    }

    void Write(void* dataToWrite, const uint32_t blockNumber)
    {
        memcpy( &data[blockNumber], dataToWrite, BlockSize );
    }

    void Erase(const uint32_t blockNumber)
    {
        memset( &data[blockNumber], 0xff, BlockSize );
    }

private:
    uint8_t     data[BlockSize*numberOfBlocks];

};





//
// PersistentCircularBuffer stores a number of elements in a specified BlockDevice. The type of 
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
template <typename BlockDeviceType, typename ContainedType, uint32_t NumberOfElements>
class PersistentCircularBuffer
{

public:

    //
    // Initialise the member-reference to the BlockDevice. This allows the optimiser to directly
    // reference the BlockDevice instead of using an indirection (pointer), thus making it safer
    // and more efficient.
    //
    PersistentCircularBuffer( BlockDeviceType& _blockdevice ) : 
        blockDevice(_blockdevice),
        firstElement(0),
        lastElement(0),
        cursorPosition(0)
    {
        //
        // - Perform search for last-good-position if not supplied from checkpoint data.
        // - Perform a integrity check of the cache. If we have good contents, use it.
        //
    }


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

    void Append()
    {
        lastElement     = (lastElement + 1) % NumberOfElements;
        MoveToLast();
    }

    void RemoveFirst()
    {
        firstElement    = (firstElement + 1) % NumberOfElements;
    }


    void Read( ContainedType& data )
    {
        blockDevice.Read( &data, BlockNumberFromElementNumber(cursorPosition) );
    }

    void Write( ContainedType& data )
    {
        blockDevice.Write( &data, BlockNumberFromElementNumber(cursorPosition) );
    }


    void ReadPartial( void* data, const size_t offset, const size_t dataLength )
    {
        blockDevice.ReadPartial( data, BlockNumberFromElementNumber(cursorPosition), offset, dataLength );
    }

    void WritePartial( void* data, const size_t offset, const size_t dataLength )
    {
        blockDevice.WritePartial( data, BlockNumberFromElementNumber(cursorPosition), offset, dataLength );
    }


    template <typename FieldType> void ReadField( FieldType& data, const size_t offset )
    {
        blockDevice.ReadPartial( &data, BlockNumberFromElementNumber(cursorPosition), offset, sizeof(data) );
    }

    template <typename FieldType> void WriteField( FieldType& data, const size_t offset )
    {
        blockDevice.WritePartial( &data, BlockNumberFromElementNumber(cursorPosition), offset, sizeof(data) );
    }


    void WriteSetComplete()
    {
        lastUpdatedCache    = (lastUpdatedCache+1) & 0x01;  // simply flip-flop between 0 and 1.
    }

private:

    typedef struct
    {
        uint32_t        sequenceNumber;         // always incrementing and identifies latest data. This value gets stored in checkpoint CB.
        uint32_t        integrityCheckValue;    // CRC32/SHA of the element contents. Used for bad-block management and reset cases.

        ContainedType   data;

    } Element;


    uint32_t BlockNumberFromElementNumber( const uint32_t elementNumber )
    {
        const uint32_t elementsPerBlock  = blockDevice.GetBlockSize() / sizeof(Element); 
        return elementNumber / elementsPerBlock;
    }

    BlockDeviceType&    blockDevice;
    uint32_t            lastElement;        // 
    uint32_t            firstElement;       // index of last-written element in the buffer.
    uint32_t            cursorPosition;     // Current position for read/write.
    
    //
    // These elements form the cache. For reset-robustness, they must be placed into a non-initialised region.
    // This data gets flushed to FLASH on shutdown.
    //
    Element             cache[2];           // Cache of latest element. This is an array of 2 in order to provide reset-robustness (double buffer).
    uint32_t            cachedElement;      // Index of cached element.
    uint32_t            lastUpdatedCache;   // index into 'cache' of last written element.
};































//
// Configuration of types.
//

typedef struct 
{
    uint32_t    fieldA;
    uint64_t    fieldB;
    bool        fieldC;

} MyData;


typedef struct 
{
    uint32_t    lastKnownGoodPositionOfDemoCircularBuffer;
    uint32_t    lastKnownGoodPositionOfConsumptionCircularBuffer;
    uint32_t    lastKnownGoodPositionOfBillingCircularBuffer;
    uint32_t    lastKnownGoodPositionOfSecurityLogCircularBuffer;
    uint32_t    lastKnownGoodPositionOfEventLogCircularBuffer;

} MyCheckpointData;


typedef RAMBlockDevice<4096, 16>     DemoBlockDeviceType; 
typedef RAMBlockDevice<4096, 4>      CheckpointBlockDeviceType; 

typedef PersistentCircularBuffer<DemoBlockDeviceType, MyData, 32>                   DemoCircularBufferType; 
typedef PersistentCircularBuffer<CheckpointBlockDeviceType, MyCheckpointData, 8>    CheckpointCircularBufferType; 


//
// Instantiation of types.
//
static DemoBlockDeviceType          demoBlockDevice;
static CheckpointBlockDeviceType    checkpointBlockDevice;

static DemoCircularBufferType       demoCircularBuffer(demoBlockDevice);
static CheckpointCircularBufferType checkpointCircularBuffer(checkpointBlockDevice);




//
// DataModel interface. Key points here:
// - Can be auto-generated to a large extent using pycparser and a struct definition (not similarity of accessors).
// - All data in the system passes through this class.
// - 
//
template <typename CircularBufferType>
class DataModel
{
public:

    DataModel( CircularBufferType& _circularBuffer ) : 
        circularBuffer(_circularBuffer)
    {
    }


    void SetFieldA(uint32_t value)
    {
        circularBuffer.WritePartial( &value, offsetof(MyData, fieldA), sizeof(value) ); 
    }

    void GetFieldA(uint32_t& value)
    {
        circularBuffer.ReadPartial( &value, offsetof(MyData, fieldA), sizeof(value) );
    }


    void SetFieldB(uint64_t value)
    {
        circularBuffer.WritePartial( &value, offsetof(MyData, fieldB), sizeof(value) ); 
    }

    void GetFieldB(uint64_t& value)
    {
        circularBuffer.ReadPartial( &value, offsetof(MyData, fieldB), sizeof(value) );
    }


    void SetFieldC(bool value)
    {
        circularBuffer.WritePartial( &value, offsetof(MyData, fieldC), sizeof(value) ); 
    }

    void GetFieldC(bool& value)
    {
        circularBuffer.ReadPartial( &value, offsetof(MyData, fieldC), sizeof(value) );
    }

private:

    CircularBufferType&    circularBuffer;

};


//
//
//
int main()
{
    {
        //
        // Direct write access to the data using type-safe methods. 
        //
        uint32_t    a   = 123;
        uint64_t    b   = 321;
        bool        c   = true;

        demoCircularBuffer.Append();
        demoCircularBuffer.WriteField<uint32_t>( a, offsetof(MyData, fieldA) ); 
        demoCircularBuffer.WriteField<uint64_t>( b, offsetof(MyData, fieldB) ); 
        demoCircularBuffer.WriteField<bool>( c, offsetof(MyData, fieldC) ); 
    }

    {
        //
        // Direct read access to the circular buffer by reading whole structure.
        //
        MyData  data    = {0};
        demoCircularBuffer.Read( data );

        printf("a = %d\n",  data.fieldA );
        printf("b = %ld\n", data.fieldB );
        printf("c = %d\n",  data.fieldC );
    }

    {
        //
        // Direct read access to the circular buffer using 'partial' accessors to avoid reading whole struct.
        //
        uint32_t    a;
        uint64_t    b;
        bool        c;

        demoCircularBuffer.ReadPartial( &a, offsetof(MyData, fieldA), sizeof(a) ); 
        demoCircularBuffer.ReadPartial( &b, offsetof(MyData, fieldB), sizeof(b) ); 
        demoCircularBuffer.ReadPartial( &c, offsetof(MyData, fieldC), sizeof(c) ); 

        printf("a = %d\n", a);
        printf("b = %ld\n", b);
        printf("c = %d\n", c);
    }

    {
        //
        // Direct read access to the circular buffer using more type-safe methods.
        //
        uint32_t    a;
        uint64_t    b;
        bool        c;

        demoCircularBuffer.ReadField<uint32_t>( a, offsetof(MyData, fieldA) ); 
        demoCircularBuffer.ReadField<uint64_t>( b, offsetof(MyData, fieldB) ); 
        demoCircularBuffer.ReadField<bool>( c, offsetof(MyData, fieldC) ); 

        printf("a = %d\n", a);
        printf("b = %ld\n", b);
        printf("c = %d\n", c);
    }


    {
        //
        // DataModel accessors.
        // DataModel can be created with read-only or read-write BlockDevices as necessary.
        //
        uint32_t    a;
        uint64_t    b;
        bool        c;

        static DataModel<DemoCircularBufferType>   dataModel(demoCircularBuffer);
        dataModel.GetFieldA(a);
        dataModel.GetFieldB(b);
        dataModel.GetFieldC(c);

        printf("a = %d\n", a);
        printf("b = %ld\n", b);
        printf("c = %d\n", c);
    }
}


