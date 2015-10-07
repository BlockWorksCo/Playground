



#ifndef __DATAMODELUPGRADE_H__
#define __DATAMODELUPGRADE_H__

#include <stdint.h>
#include <stdbool.h>


//
// Old version of DataModelMap (Note the re-ordered fields when compared to DataModelMap).
//
typedef struct
{
    uint8_t     version;

    uint32_t    four;
    uint32_t    three;
    uint32_t    two;
    uint32_t    one;

} DataModelMap_v0;


void DataModelUpgrade();


#endif


