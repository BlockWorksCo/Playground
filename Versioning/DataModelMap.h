


#ifndef __DATAMODELMAP_H__
#define __DATAMODELMAP_H__



//
//
//
typedef struct
{
    uint8_t     version;

    uint32_t    one;
    uint32_t    two;
    uint32_t    three;
    uint32_t    four;
    uint8_t     queueA[512];

} DataModelMap;



//
// TODO: Put into Common.h
//
#define sizeoffield(type, field) sizeof(((type *)0)->field)



#endif


