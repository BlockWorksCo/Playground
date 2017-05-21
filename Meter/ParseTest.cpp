

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>







//
//
//
class Object
{

public:

    void Get(uint16_t attributeNumber);
    void Set(uint16_t attributeNumber);
    void Read(uint16_t attributeNumber);
    void Write(uint16_t attributeNumber);

private:


};



//
//
//
class Attribute
{

public:

    void Get();
    void Set();
    void Read();
    void Write();

private:


};





//
//
//
class LogicalNameAttribute : public Attribute
{

public:

    void Get();
    void Set();
    void Read();
    void Write();

private:


};



//
//
//
class TimeAttribute : public Attribute
{

public:

    void Get();
    void Set();
    void Read();
    void Write();

private:


};



//
//
//
class TimeZoneAttribute : public Attribute
{

public:

    void Get();
    void Set();
    void Read();
    void Write();

private:


};


//
//
//
class Clock : public Object
{

public:

    void Get(uint16_t attributeNumber);
    void Set(uint16_t attributeNumber);
    void Read(uint16_t attributeNumber);
    void Write(uint16_t attributeNumber);

private:

    LogicalNameAttribute    attribute0;
    TimeAttribute           attribute8;
    TimeZoneAttribute       attribute16;


};




//
//
//
typedef uint64_t    OBISCode;


//
//
//
class ObjectList
{

public:

    void Get(OBISCode code, uint16_t attributeNumber)
    {
        clock.Get(attributeNumber);
    }

    void Set(OBISCode code, uint16_t attributeNumber)
    {
        clock.Set(attributeNumber);
    }

    void Read(OBISCode code, uint16_t attributeNumber)
    {
        clock.Read(attributeNumber);
    }

    void Write(OBISCode code, uint16_t attributeNumber)
    {
        clock.Write(attributeNumber);
    }

    


private:

    Clock   clock;

};




//
//
//
template <typename T> 
class Stream
{

public:

    Stream( T* _data, uint32_t _numberOfValues ) :
        data(_data),
        numberOfValues(_numberOfValues),
        currentPosition(0)
    {
    }
    
    bool Get(T& value)
    {
        if(currentPosition < numberOfValues)
        {
            value   = data[currentPosition];
            currentPosition++;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Put(T& value)
    {
        printf("%02x", value);
    }

private:

    T*          data;
    uint32_t    numberOfValues;
    uint32_t    currentPosition;

};






//
//
//
typedef struct
{
    uint32_t     length;
    uint8_t      octets[128];

} OctetString;


//
//
//
typedef enum
{
    Long            = 0,
    UnsignedLong    = 1,
    Integer         = 2,
    Unsigned        = 3,
    Octet           = 4,
    Array           = 5,

} Type;

uint32_t    sizeOfType[]    =
{
    sizeof(int32_t),
    sizeof(uint32_t),
    sizeof(int16_t),
    sizeof(uint16_t),
    sizeof(uint8_t),
    0,
};


#define MAX_STACK_DEPTH         (16)
#define MAX_ELEMENTS_PER_MAP    (16)

//
//
//
typedef struct
{
    Type        type;
    uint32_t    offset;
    uint32_t    n;
    
} ParseElement;

//
//
//
typedef struct
{
    ParseElement    elements[MAX_ELEMENTS_PER_MAP];
    uint32_t        currentElement;
    uint32_t        currentOffset;
    void*           map;

} ParsingContext;



//
//
//
typedef struct
{
    ParsingContext  contexts[MAX_STACK_DEPTH];
    uint32_t        top;

} ParsingStack;



//
//
//
void ParseByte( ParsingContext& context, uint8_t* byte )
{
    if(context.currentOffset == 0)
    {
        void**  mapPtr  = (void**)(context.map + context.elements[context.currentElement].offset);
        *mapPtr = byte;
    }

    context.currentOffset++;

    if(context.currentOffset >= sizeOfType[context.elements[context.currentElement].type] )
    {
        context.currentOffset   = 0;
        context.currentElement++;
    }
}




//
//
//
typedef struct
{
    uint8_t*    stringLength;
    uint8_t*    text;

} ClockAttribute2Response;

ClockAttribute2Response clockAttribute2ResponseMap;
ParsingContext  ClockAttribute2ResponseContext  =
{
    .elements       =
    {
        {.type=Octet,   .offset=offsetof(ClockAttribute2Response, stringLength),    .n=1},
        {.type=Octet,   .offset=offsetof(ClockAttribute2Response, text),            .n=32},
    },
    .currentElement = 0,
    .currentOffset  = 0,
    .map    = &clockAttribute2ResponseMap,
};



//
//
//
template <typename StreamType>
class DLMSStack
{

public:

    DLMSStack(StreamType& _stream) :
        stream(_stream),
        startReceived(false)
    {
    }

    
    bool Iterate()
    {
        uint8_t value;
        bool result = stream.Get(value);
        if(result == true)
        {
            //
            // Delimit the messages.
            //
            if( (value == 0x7e) && (startReceived == false) )
            {
                byteIndex       = 0;
                startReceived   = true;
            }
            else if( (value == 0x7e) && (startReceived == true) )
            {
                startReceived   = false;
            }

            //
            // Parse the important bytes in the stream.
            //
            switch(byteIndex)
            {
                case 11:
                    printf("Request: %02x\n", value);
                    break;

            }

            return true;
        }
        else
        {
            return false;
        }
    }

private:

    StreamType  stream;
    uint32_t    byteIndex;
    bool        startReceived;

    ObjectList  objectList;

};


//
//
//
int main()
{
    uint8_t                     request[]           = {0x7E,0xA0,0x19,0x95,0x75,0x54,0x68,0x35,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x08,0x00,0x00,0x01,0x00,0x00,0xFF,0x01,0x00,0x0D,0xFD,0x7E};
    Stream<uint8_t>             stream( &request[0], sizeof(request) );
    DLMSStack<Stream<uint8_t> > dlmsStack(stream);
    bool                        result;

    do
    {
        result = dlmsStack.Iterate();

    } while(result == true);

}



