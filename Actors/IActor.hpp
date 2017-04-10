





#ifndef __IACTOR_HPP__
#define __IACTOR_HPP__


#include <stdint.h>


//
// NOTE: This isn't a *true* actor implementation, its effectively a lightweight concurrency
// mechanism to reduce the need for tasks instead... Don't shoot me. ;o) (SteveT)
//
template <typename DataType>
class IActor
{
public:

   virtual void ProcessMessage(uint32_t id, DataType data) = 0;
};



#endif


