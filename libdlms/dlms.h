

#ifndef __DLMS_H__
#define __DLMS_H__


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Stream.h"

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


typedef uint8_t     OBISCode[6];
typedef uint16_t    AttributeId;


typedef enum
{
// standardized DLMS PDUs used in COSEM
// DLMS PDUs (no encryption selected 24)
  initiateRequest          =1,
  readRequest              =5,
  writeRequest             =6,
  initiateResponse         =8,
  readResponse             =12,
  writeResponse            =13,
  confirmedServiceError    =14,
  unconfirmedWriteRequest  =22,
  informationReportRequest =24,
  
  glo_initiateRequest      =33,
  glo_readRequest          =37,
  glo_writeRequest         =38,
  
  glo_initiateResponse     =40,
  glo_readResponse         =44,
  glo_writeResponse        =45,

// the four ACSE APDUs
//  aarq AARQ-apdu
//  aare AARE-apdu
//  rlrq RLRQ-apdu -- -- OPTIONAL
//  rlre RLRE-apdu -- -- OPTIONAL
  aarq                      = 96,
  aare                      = 97,

// APDUs used for data communication services using LN referencing
  get_request                =192,
  set_request                =193,
  event_notification_request =194,
  action_request             =195,
  get_response               =196,
  set_response               =197,
  action_response            =199,

// global ciphered pdus
  glo_get_request                =200,
  glo_set_request                =201,
  glo_event_notification_request =202,
  glo_action_request             =203,
  glo_get_response               =204,
  glo_set_response               =205,
  glo_action_response            =207,

// dedicated ciphered pdus
  ded_get_request                =208,
  ded_set_request                =209,
  ded_event_notification_request =210,
  ded_actionRequest              =211,
  ded_get_response               =212,
  ded_set_response               =213,
  ded_action_response            =215,

// the exception response pdu 
  exception_response             =216,
} PDUType;


// Used in the InitiateRequest APDU (AARQ).
typedef enum
{
    reserved0                     =0,
    reserved1                     =1,
    reserved2                     =2,
    read                          =3,
    write                         =4,
    unconfirmed_write             =5,
    reserved6                     =6,
    reserved7                     =7,
    attribute0_supported_with_SET =8,
    priority_mgmt_supported       =9,
    attribute0_supported_with_GET =10,
    block_transfer_with_get       =11,
    block_transfer_with_set       =12,
    block_transfer_with_action    =13,
    multiple_references           =14,
    information_report            =15,
    reserved16                    =16,
    reserved17                    =17,
    parameterized_access          =18,
    get                           =19,
    set                           =20,
    selective_access              =21,
    event_notification            =22,
    action                        =23,

} ConformanceBits;


void dlmsFormGetRequest( Stream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId );
void dlmsParseGetRequest( Stream* stream,  OBISCode* obisCode, InterfaceClass* ifClass, AttributeId* attributeId );
void dlmsFormGetResponseNormal( Stream* stream,  ResultType type );
void dlmsParseGetResponseNormal( Stream* stream,  ResultType* resultType );
void dlmsFormSetRequest( Stream* stream,  OBISCode obisCode, InterfaceClass ifClass, AttributeId attributeId );
void dlmsParseSetRequest( Stream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId );
void dlmsFormSetResponse( Stream* stream,  DataAccessResult result );
void dlmsParseSetResponse( Stream* stream, DataAccessResult* result );
void dlmsFormAttributeDescriptor( Stream* stream, InterfaceClass ifClass, OBISCode obisCode, AttributeId attrId );
void dlmsParseAttributeDescriptor( Stream* stream, InterfaceClass* ifClass, OBISCode* obisCode, AttributeId* attrId );

void dlmsParseAccessSelection( Stream* stream, bool* accessSelection, uint8_t* accessSelector );
void dlmsFormNoAccessSelection( Stream* stream );

void dlmsFormByTimeRangeAccessSelection( Stream* stream, uint32_t from, uint32_t to );
void dlmsParseByTimeRangeAccessSelection( Stream* stream, uint32_t* from, uint32_t* to );

void dlmsFormByEntryAccessSelection( Stream* stream, uint32_t from, uint32_t to );
void dlmsParseByEntryAccessSelection( Stream* stream, uint32_t* from, uint32_t* to );

void dlmsFormTimeFromUTC( Stream* stream, uint32_t utc );
void dlmsParseTime( Stream* stream, uint32_t* utc );

void dlmsFormAARQ( Stream* stream );
void dlmsFormAARQForLLS( Stream* stream );
void dlmsFormAARQForHLS( Stream* stream );
void dlmsFormInitiateRequestAPDU( Stream* stream,  uint8_t dedicatedKey[],uint32_t dedicatedKeyLength, bool responseAllowed, int8_t proposedQualityOfService, uint8_t proposedDlmsVersion, uint32_t proposedConformanceFlags, uint16_t clientMaxReceivePDUSize );
void dlmsParseAARQ( Stream* stream, char* mechanismName, uint32_t mechanismNameMaxSize );
void dlmsFormAARE( Stream* stream, uint8_t appContextName[],uint32_t appContextNameLength, uint8_t respondingAPTitle[],uint32_t respondingAPTitleLength, uint8_t initiateRequestAPDU[], uint32_t initiateRequestAPDULength );  
void dlmsParseAARE( Stream* stream, uint8_t* appContextName, uint32_t* appContextNameLength, uint8_t* respondingAPTitle, uint32_t* respondingAPTitleLength, uint8_t* initiateRequestAPDU,uint32_t* initiateRequestAPDULength  );



#endif

