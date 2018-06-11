

#include "axdr.h"
#include "dlms.h"




//
//
//
void dlmsFormAttributeDescriptor( AXDRStream* stream, InterfaceClass ifClass, OBISCode obisCode, AttributeId attrId )
{
    uint16_t    ic  = ifClass;
    axdrSetUint8Array( stream, (void*)&ic, sizeof(ic) );

    axdrSetUint8Array( stream, obisCode,sizeof(OBISCode) );
    
    axdrSetUint8( stream, attrId );
}

void dlmsParseAttributeDescriptor( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId )
{
    uint16_t ic;
    axdrGetUint8Array( stream, (void*)&ic,sizeof(ic) );
    *ifClass    = (InterfaceClass)ic;

    axdrGetUint8Array( stream, (void*)obisCode,sizeof(OBISCode) );

    uint8_t attr;
    axdrGetUint8( stream, &attr );
    *attrId  = (AttributeId)attr;
}


