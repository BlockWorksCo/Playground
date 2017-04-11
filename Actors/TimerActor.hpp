


#ifndef __TIMERACTOR_HPP__
#define __TIMERACTOR_HPP__



#include "Actor.hpp"



//
// Example of an Actor that can provides a timing service to other actors and is updated via 
// an ISR.
// This effectively provides an application-level timing facility by allowing actors to be triggered
// periodically.
//
template <typename ActorTaskType, typename DataType>
class Timer : public Actor<ActorTaskType,DataType>
{
public:
   Timer(ActorTaskType& _task) :
         Actor<ActorTaskType,DataType>(_task),
         tick(0)
   {      
      for(uint32_t i=0; i<8; i++)
      {
         periodicTimers[i]    = 0;
      }
   }

   void ProcessMessage(uint32_t id, DataType data)
   {
      //
      // The message will be either an 'update-tick' message from an ISR or a 
      // 'register' message that allows another actor to request itself to be called repeatedly.
      //
      switch(id)
      {
         case Tick:     // increment a tick (send via timer ISR)
            tick++;
            break;

         case Register:     // register an actor that wishes a delay.
            printf("Registration message.\n");
            for(uint32_t i=0; i<8; i++)            
            {
               if(periodicTimers[i] == 0)
               {
                  periodicTimers[i]    = (RegisterMessage*)data;
                  break;
               }
            }            
            break;

         default:
            break;
      }

      //
      // Scan thru the periodic timers to see if any are ready to fire.
      //
      for(uint32_t i=0; i<8; i++)
      {
         if(periodicTimers[i] != 0)
         {
            if( (tick % periodicTimers[i]->period) == 0)
            {
               this->Send( periodicTimers[i]->recipient, periodicTimers[i]->id, tick );
            }
         }
      }
   }


   //
   // Message type enumeration
   //
   typedef enum
   {
      Tick,
      Register
   } MessageType;

   //
   // Message structures.
   //
   typedef struct
   {
      ActorID     recipient;
      uint32_t    id;
      uint32_t    period;
   } RegisterMessage;

private:

   RegisterMessage*     periodicTimers[8];
   uint32_t             tick;
};



#endif



