//
// PersistentCircularBuffers address the following issues:
// - Wear levelling (circular buffers naturally spread the wear).
// - Bad block management (with deterministic over-allocation).
// - Integrity checked storage.
// - Encrypted storage (easy point to add encryption if needed).
// - Performance/minimisation of erases for extended lifetime.
// - Deterministic & quick shutdown performance (pre-erasing of blocks).
// - Atomic-write property (with auto-rollback after reset when integrity checking is used).
// - System-wide consistency with auto-rollback after a reset (when a checkpoint circular-buffer is maintained).
// - Reset-robustness (when BlockStorage is in a non-initialised RAM region).
//
// DataModel addresses the following issues:
// - Strong separation of concerns between application & persistence layers.
// - Sanitisation (data is either stored on-stack or in-cache, both are easily sanitisable).
// - Ease of use (Application-side usage is simply GetXYZ() and SetXYZ() ).
// - DataModel class accessor generation can be scripted.
//



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "ApplicationTypes.h"
#include "DataModel.h"
#include "PersistentCircularBuffer.h"
#include "RAMBlockStorage.h"
#include "FileBlockStorage.h"


//
// typedefs for clarity.
//
typedef RAMBlockStorage<4096, 16>     DemoBlockStorageType; 
typedef RAMBlockStorage<4096, 4>      CheckpointBlockStorageType; 
typedef RAMBlockStorage<4096, 48>     ConsumptionBlockStorageType; 

typedef PersistentCircularBuffer<DemoBlockStorageType, MyData, 32>                   DemoCircularBufferType; 
typedef PersistentCircularBuffer<CheckpointBlockStorageType, MyCheckpointData, 8>    CheckpointCircularBufferType; 


//
// Instantiation of types.
//
static DemoBlockStorageType          demoBlockStorage;
static CheckpointBlockStorageType    checkpointBlockStorage;

static DemoCircularBufferType       demoCircularBuffer(demoBlockStorage);
static CheckpointCircularBufferType checkpointCircularBuffer(checkpointBlockStorage);



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
        demoCircularBuffer.WriteSetComplete();
    }

    {
        //
        // Direct write access to the data using type-safe methods. 
        //
        uint32_t    a   = 567;
        uint64_t    b   = 765;
        bool        c   = false;
        static DataModel<DemoCircularBufferType>   dataModel(demoCircularBuffer);

        demoCircularBuffer.Append();
        dataModel.SetFieldA(a);
        dataModel.SetFieldB(b);
        dataModel.SetFieldC(c);
        demoCircularBuffer.WriteSetComplete();
    }


    //
    // The following sections are all equivalent.
    //


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
        // DataModel can be created with read-only or read-write BlockStorages as necessary.
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


