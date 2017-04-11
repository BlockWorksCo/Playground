





#ifndef __IACTOR_HPP__
#define __IACTOR_HPP__


#include <stdint.h>

//
// Up to 255 actors in one ActorTask.
//
typedef uint8_t     ActorID;

//
// Type for the message identifier.
//
typedef uint32_t     MessageID;


//
// NOTE: This isn't a *true* actor implementation, its effectively a lightweight concurrency
// mechanism to reduce the need for tasks instead... Don't shoot me. ;o) (SteveT)
//
template <typename DataType>
class IActor
{
public:

   virtual void ProcessMessage( MessageID id, DataType data ) = 0;
};



#endif


