





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
               state(0)
   {
      task.Add( *this );
   }


   void Send(uint32_t recipient, uint32_t id, DataType data)
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

   //
   // return the ID of the curerntly executing actor.
   //
   uint32_t self()
   {
      return this->task.CurrentlyExecutingActor();
   }

private:

   uint32_t          state;
   ActorTaskType&    task;
};



#endif


