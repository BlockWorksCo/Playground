


#ifndef __DATAMODEL_H__
#define __DATAMODEL_H__

#include <stdbool.h>
#include <stdint.h>



void DataModelInitialise();

void DataModelGetItemOne(uint32_t* data);
void DataModelGetItemTwo(uint32_t* data);
void DataModelGetItemThree(uint32_t* data);
void DataModelGetItemFour(uint32_t* data);

void DataModelSetItemOne(uint32_t* data);
void DataModelSetItemTwo(uint32_t* data);
void DataModelSetItemThree(uint32_t* data);
void DataModelSetItemFour(uint32_t* data);



#endif

