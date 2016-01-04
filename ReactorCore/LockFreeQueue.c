

#include <stdint.h>
#include <stdbool.h>




//
//
//
typedef struct
{
    volatile uint32_t producer;
    volatile uint32_t consumer;
    size_t            numberOfElements;
    void**            data;

} Queue;




//
//
//
void Initialise(Queue* q, size_t numberOfElements, void** data)
{
    q->numberOfElements = numberOfElements;
    q->data             = data;
    q->producer         = 0;
    q->consumer         = 0;
}





//
//
//
bool IsEmpty(Queue* q)
{
    if(q->producer == q->consumer)
    {
        return true;
    }
    else
    {
      return false;      
    }
}


//
//
//
bool IsFull(Queue* q)
{
    if(q->producer - q->consumer >= q->numberOfElements)
    {
        return true;
    }
    else
    {
        return false;      
    }
}


//
//
//
void* Get(Queue* q)
{
    //
    // Block while no elements in the queue.
    //
    while(IsEmpty(q) == true);

    //
    // Extract the data from the queue and move the pointer around the queue.
    //
    void*   ret = q->data[q->consumer % q->numberOfElements];
    q->consumer++;

    return ret;
}


//
//
//
void Put(Queue* q, void* item)
{
    //
    // Block while there is no space in the queue.
    //
    while(IsFull(q) == true);

    //
    // Put the data in the queue and move the pointers around.
    //
    q->data[q->producer % q->numberOfElements] = item;
    q->producer++;
}


