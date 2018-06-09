

#ifndef __DLMS_H__
#define __DLMS_H__


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef enum
{
    TimeClass   = 8,

} InterfaceClass;


typedef enum
{
    Data            = 0,
    AccessResult    = 1, 

} ResultType;


typedef enum
{
    success                   = 0,
    hardware_fault            = 1,
    temporary_failure         = 2,
    read_write_denied         = 3,
    object_undefined          = 4,
    object_class_inconsistent = 9,
    object_unavailable        = 11,
    type_unmatched            = 12,
    scope_of_access_violated  = 13,
    data_block_unavailable    = 14,
    long_get_aborted          = 15,
    no_long_get_in_progress   = 16,
    long_set_aborted          = 17,
    no_long_set_in_progress   = 18,
    other_reason              = 250

} DataAccessResult;


typedef enum
{
    NoSelectiveAccess,

} SelectiveAccessType;

typedef uint8_t     OBISCode[6];
typedef uint16_t    AttributeId;



void dlmsFormGetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId );
void dlmsParseGetRequest( AXDRStream* stream,  OBISCode* obisCode, InterfaceClass* ifClass, AttributeId* attributeId );
void dlmsFormGetResponseNormal( AXDRStream* stream,  ResultType type );
void dlmsParseGetResponseNormal( AXDRStream* stream,  ResultType* resultType );
void dlmsFormSetRequest( AXDRStream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId );
void dlmsFormSelectiveAccessType( AXDRStream* stream, SelectiveAccessType type);
void dlmsParseSetRequest( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId, SelectiveAccessType* saType );
void dlmsParseAccessSelection( AXDRStream* stream, uint8_t* accessSelector );
void dlmsFormAttributeDescriptor( AXDRStream* stream, InterfaceClass ifClass, OBISCode obisCode, AttributeId attrId );
void dlmsParseAttributeDescriptor( AXDRStream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId );

#endif

