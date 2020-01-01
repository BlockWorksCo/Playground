//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __QUEUE_H__
#define __QUEUE_H__




#define DECLARE_QUEUE(name, type, size) 		\
type name##Data[size];\
uint8_t name##Head;\
uint8_t name##Tail;\
void name##Put( type element )\
{\
    uint8_t    newTail   = (name##Tail+1) % size;\
    if(newTail != name##Head)\
    {\
        name##Data[name##Tail]  = element;\
        name##Tail   = newTail;\
    }\
    else\
    {\
        PANIC();\
    }\
}\
type name##Get(type noneValue)\
{\
	type 	c;\
    if (name##Head != name##Tail) \
    {\
        c = name##Data[name##Head];\
        name##Head = (name##Head + 1) % size;\
    } \
    else \
    {\
    	c = noneValue;\
    }\
    return c;\
}

#endif



