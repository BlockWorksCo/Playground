





#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__




#include "IActor.hpp"
#include "ActorTask.hpp"
#include <stdint.h>
#include <stdbool.h>


//
// NOTE: This isn't a *true* actor implementation, its effectively a lightweight concurrency
// mechanism to reduce the need for tasks instead... Don't shoot me. ;o) (SteveT)
//
template <typename ActorTaskType, typename DataType>
class Actor : public IActor<DataType>
{
public:

   Actor(ActorTaskType& _task) :
               task(_task), 
               state(0),
               self( task.Add(*this) )
   {
   }


   void Send( ActorID recipient, MessageID id, DataType data)
   {
      task.Send(recipient, id, data);
   }


protected:

   //
   // State management
   //
   void SetState(uint32_t newState)
   {
      state    = newState;
   }

   uint32_t GetState()
   {
      return state;
   }

   ActorID           self;

private:

   uint32_t          state;
   ActorTaskType&    task;
};



#endif


