
#ifndef __CIRCULARBUFFER_HPP__
#define __CIRCULARBUFFER_HPP__


#include <stdint.h>



//
//
//
template <typename ContainedType, typename IndexType>
class CircularBuffer
{
public:

    void InitialiseAsReader()
    {
        tail    = 0;
        head    = 1;
        numberOfReaders++;
    }

    void InitialiseAsWriter()
    {
        tail    = 0;
        head    = 1;
        numberOfWriters++;
    }

    inline void Put(ContainedType value)
    {
        IndexType    newHead     = head + 1;

        //
        // Wait until there is space in the buffer.
        //
        while(newHead == tail);
    
        //
        // Put the data in the buffer.
        //
        data[head]  = value;
        head    = newHead;
    }


    inline ContainedType Get()
    {
        IndexType   newTail     = tail + 1;

        //
        // Wait until there is data in the buffer.
        //
        while(head == tail);

        //
        // Get the data out of the buffer.
        //
        ContainedType     value   = data[tail];
        tail    = newTail;

        return value;
    }

    ContainedType NonBlockingGet(bool& dataAvailable)
    {

        //
        // Wait until there is data in the buffer.
        //
        if(head == tail)
        {
            dataAvailable   = false;
            return 0;
        }
        else
        {
            //static IndexType   newTail     = tail + 1;
            //
            // Get the data out of the buffer.
            //
            ContainedType     value   = data[tail];
            //tail    = newTail;
            tail++;
            dataAvailable   = true;

            return value;
        }

    }


private:

    volatile ContainedType   data[1<<(sizeof(IndexType)*8)];
    volatile IndexType       head;
    volatile IndexType       tail;
    volatile uint32_t        numberOfReaders;
    volatile uint32_t        numberOfWriters;

};


#endif


