
#ifndef __APPLICATIONTYPES_H__
#define __APPLICATIONTYPES_H__


#include <stdint.h>
#include <stdbool.h>


//
// The DataModel contains all the data in the system. All data is accessed through this class(es).
//
// Although its tempting to group fields by functional-area (i.e. ZigBee, Auth, Billing, etc). It should actually
// be grouped according to access-pattern (OneHz, half-hourly, daily, monthly, yearly, etc).
// The application should *not* be specifying the access-pattern and required performance characteristics (i.e. "RAM backup").
// This is because this will vary greatly between variants/products and can only be done correctly in a later performance-tuning
// stage.
//



typedef struct 
{
    uint32_t    fieldA;
    uint64_t    fieldB;
    bool        fieldC;

} MyData;


typedef struct 
{
    uint32_t    lastKnownGoodPositionOfDemoCircularBuffer;
    uint32_t    lastKnownGoodPositionOfConsumptionCircularBuffer;
    uint32_t    lastKnownGoodPositionOfBillingCircularBuffer;
    uint32_t    lastKnownGoodPositionOfSecurityLogCircularBuffer;
    uint32_t    lastKnownGoodPositionOfEventLogCircularBuffer;

} MyCheckpointData;




#endif


