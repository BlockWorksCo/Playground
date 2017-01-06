

#ifndef __DATAMODEL_H__
#define __DATAMODEL_H__

#include <stdint.h>
#include <stdbool.h>
#include <cstddef>



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

    //
    // Accessors (auto-generated?)
    //
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

    //
    // serialisation (for version migration).
    // Store the fields in such a way that they are offset-independent (i.e not packed in a binary structure).
    // Suggest to use some KLV (Key,Length,Value) type scheme.
    //
    // The critical point here is that the fields can be individually acessed for backup/restore purposes. This
    // is only done during the upgrade activation phase.
    //
    void Serialise()
    {        
        MyData  data;

        circularBuffer.Read( data );
        SerialiseField( "fieldA", sizeof(data.fieldA), &data.fieldA );
        SerialiseField( "fieldB", sizeof(data.fieldB), &data.fieldB );
        SerialiseField( "fieldC", sizeof(data.fieldB), &data.fieldC );
    }

    void Deserialise()
    {        
        MyData  data;

        DeserialiseField( "fieldA", sizeof(data.fieldA), &data.fieldA );
        DeserialiseField( "fieldB", sizeof(data.fieldB), &data.fieldB );
        DeserialiseField( "fieldC", sizeof(data.fieldB), &data.fieldC );
        circularBuffer.Write( data );
    }

private:

    //
    // Field serialisation methods.
    //
    uint32_t KeyFromFieldName(const char* fieldName)
    {
        //return HASH(fieldName);
        return 0;
    }

    void SerialiseField(const uint32_t key, const uint32_t length, uint8_t* value)
    {

    }

    void DeserialiseField(const uint32_t key, const uint32_t length, uint8_t* value)
    {

    }


    CircularBufferType&    circularBuffer;

};




#endif

