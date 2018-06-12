

#include "axdr.h"
#include "dlms.h"




//
//
//
void dlmsFormAttributeDescriptor( Stream* stream, InterfaceClass ifClass, OBISCode obisCode, AttributeId attrId )
{
    uint16_t    ic  = ifClass;
    streamSetUint8Array( stream, (void*)&ic, sizeof(ic) );

    streamSetUint8Array( stream, obisCode,sizeof(OBISCode) );
    
    streamSetUint8( stream, attrId );
}

void dlmsParseAttributeDescriptor( Stream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId )
{
    uint16_t ic;
    streamGetUint8Array( stream, (void*)&ic,sizeof(ic) );
    *ifClass    = (InterfaceClass)ic;

    streamGetUint8Array( stream, (void*)obisCode,sizeof(OBISCode) );

    uint8_t attr;
    streamGetUint8( stream, &attr );
    *attrId  = (AttributeId)attr;
}


