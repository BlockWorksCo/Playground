

#include "ActorTask.hpp"
#include "Actor.hpp"
#include "TimerActor.hpp"
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



