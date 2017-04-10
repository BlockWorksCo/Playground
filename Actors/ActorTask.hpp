





#ifndef __ACTORTASK_HPP__
#define __ACTORTASK_HPP__


#include "IActor.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>




//
// https://en.wikipedia.org/wiki/Actor_model
// "The actor model is characterized by inherent concurrency of computation within and among actors, 
//  dynamic creation of actors, inclusion of actor addresses in messages, and interaction only through 
//  direct asynchronous message passing with no restriction on message arrival order."
//
// (Here, we ignore the "dynamic creation"" part of that statement).
//
// Currently this is suitable for bare-metal... i.e. bare-metal-actors.... ooh-err.
//
template <uint32_t maxNumberOfActors, uint32_t maxNumberOfMessages, typename DataType>
class ActorTask
{
public:

   explicit ActorTask(const char* name,
                      uint32_t stackSize,
                      uint32_t priority) :
               numberOfActors(0),
               queueHead(0)
   {      
   }

   void Add(IActor<DataType>& actor)
   {
      actors[numberOfActors]  = &actor;
      numberOfActors++;
   }


   //
   // Forever.... (call this method in a while(true) loop).
   // - wait for a message.
   // - send message to actor.
   //       
   void run()
   {   
      uint32_t    message     = select();
      if( message != (uint32_t)-1 )
      {
         //
         // Process a message.
         //
         currentActor   = messages[message].recipient;
         messages[message].recipient   = (ActorID)-1;
         actors[currentActor]->ProcessMessage( messages[message].id, messages[message].data );         
         currentActor   = (ActorID)-1;
      }
      else
      {
         //
         // No messages... so go to sleep until an interrupt wakes us and places
         // a message in the Q.
         //
      }
   }


   void Send(ActorID recipient, uint32_t id, DataType data)
   {
      //
      // TODO: Make this a true circular buffer.
      //
      for(uint32_t i=0; i<maxNumberOfMessages; i++)
      {
         if(messages[i].recipient >= maxNumberOfActors)
         {
            messages[i].recipient   = recipient;
            messages[i].id          = id;
            messages[i].data        = data;
            break;
         }
      }
   }

   void SendFromISR(ActorID recipient, uint32_t id, uint32_t data)
   {
      //
      // TODO: Use a separate queue and process them both in the run-loop to avoid blocking or locking.
      //
      Send(recipient, id, data);
   }

   //
   // return the currently executing actor.
   //
   ActorID CurrentlyExecutingActor()
   {
      return currentActor;
   }

private:

   //
   // 
   //
   typedef struct
   {
      ActorID     recipient;
      uint32_t    id;
      DataType    data;
      
   } Message;

   //
   // TODO: Make this a true circular buffer.
   //
   uint32_t select()
   {
      for(uint32_t i=0; i<maxNumberOfMessages; i++)
      {
         if(messages[queueHead].recipient < numberOfActors)
         {
            return queueHead;
         }

         queueHead   = (queueHead+1) % maxNumberOfMessages;
      }
      
      return (uint32_t)-1;
   }

   //
   //
   //
   IActor<DataType>*    actors[maxNumberOfActors];
   ActorID              numberOfActors;
   uint32_t             queueHead;
   Message              messages[maxNumberOfMessages];
   ActorID              currentActor;

};



#endif


