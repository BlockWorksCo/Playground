

#include "ActorTask.hpp"
#include "Actor.hpp"
#include <stdio.h>


//
//
//
typedef enum
{
   ActorTimer  = 0,
   ActorOne    = 1,
   ActorTwo    = 2,

} ActorName;



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





//
//
//
template <typename ActorTaskType, typename DataType>
class One : public Actor<ActorTaskType,DataType>
{
public:
   One(ActorTaskType& _task) :
      Actor<ActorTaskType,DataType>(_task)
   {      
      static typename Timer<ActorTaskType,DataType>::RegisterMessage    periodicTimer  =
      {
         .recipient  = ActorOne,
         .id         = 123,
         .period     = 1000,
      };
      this->Send( ActorTimer, Timer<ActorTaskType,DataType>::Register, (DataType)&periodicTimer );
   }

   void ProcessMessage(uint32_t id, DataType data)
   {
      this->Send( ActorTwo, 0xabcd, 100);
      printf("One (%d)\n", this->GetState());
      this->SetState( this->GetState()+1 );
   }
};




//
//
//
template <typename ActorTaskType, typename DataType>
class Two : public Actor<ActorTaskType,DataType>
{
public:
   Two(ActorTaskType& _task) :
      Actor<ActorTaskType,DataType>(_task)
   {      
   }

   void ProcessMessage(uint32_t id, DataType data)
   {
      //this->Send( ActorOne, 0x0123, 200);
      printf("Two\n");

      this->SetState(2);
   }
};


//
//
//
int main()
{
    typedef uint64_t                    DataType;
    typedef ActorTask<3,16,DataType>    ActorTaskType;

    ActorTaskType                       actorTask("ActorDemo",1024,1);
    Timer<ActorTaskType,DataType>       timer(actorTask);
    One<ActorTaskType,DataType>         one(actorTask);
    Two<ActorTaskType,DataType>         two(actorTask);

   while(true)
   {
      actorTask.run();

      //
      // Simulated timer interrupt.
      //
      static uint64_t   i=0;
      i++;
      if( (i%10000) == 0)
      {
         actorTask.SendFromISR( ActorTimer, Timer<ActorTaskType,DataType>::Tick, 0 );
      }
   }
}



