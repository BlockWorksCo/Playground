



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
        // Perform search for last-good-position if not supplied.
        // If the integrity check succeeds here then we have a BlockDevice with pre-existing data 
        // which is handy in the case of a watchdog reset.
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

private:

    typedef struct
    {
        uint32_t        sequenceNumber;
        uint32_t        integrityCheckValue;

        ContainedType   data;

    } Element;


    uint32_t BlockNumberFromElementNumber( const uint32_t elementNumber )
    {
        const uint32_t elementsPerBlock  = blockDevice.GetBlockSize() / sizeof(Element); 
        return elementNumber / elementsPerBlock;
    }

    BlockDeviceType&    blockDevice;
    uint32_t            lastElement;
    uint32_t            firstElement;
    uint32_t            cursorPosition;
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
typedef PersistentCircularBuffer<DemoBlockDeviceType, MyData, 32>  DemoCircularBufferType; 

//
// Instantiation of types.
//
static DemoBlockDeviceType      blockDevice;
static DemoCircularBufferType   circularBuffer(blockDevice);




//
//
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

        circularBuffer.Append();
        circularBuffer.WriteField<uint32_t>( a, offsetof(MyData, fieldA) ); 
        circularBuffer.WriteField<uint64_t>( b, offsetof(MyData, fieldB) ); 
        circularBuffer.WriteField<bool>( c, offsetof(MyData, fieldC) ); 
    }

    {
        //
        // Direct read access to the circular buffer by reading whole structure.
        //
        MyData  data    = {0};
        circularBuffer.Read( data );

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

        circularBuffer.ReadPartial( &a, offsetof(MyData, fieldA), sizeof(a) ); 
        circularBuffer.ReadPartial( &b, offsetof(MyData, fieldB), sizeof(b) ); 
        circularBuffer.ReadPartial( &c, offsetof(MyData, fieldC), sizeof(c) ); 

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

        circularBuffer.ReadField<uint32_t>( a, offsetof(MyData, fieldA) ); 
        circularBuffer.ReadField<uint64_t>( b, offsetof(MyData, fieldB) ); 
        circularBuffer.ReadField<bool>( c, offsetof(MyData, fieldC) ); 

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

        static DataModel<DemoCircularBufferType>   dataModel(circularBuffer);
        dataModel.GetFieldA(a);
        dataModel.GetFieldB(b);
        dataModel.GetFieldC(c);

        printf("a = %d\n", a);
        printf("b = %ld\n", b);
        printf("c = %d\n", c);
    }
}


