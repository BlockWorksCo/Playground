

#include "axdr.h"
#include "dlms.h"



//
// c001810007 0100620100ff 02 
//   full/NoSA --> 00
//   by entry   --> 01 02 0204 0600000005 0600000005 120001 120013 
//   by range  -->
//
// By range:
// <AccessSelection>
//      <AccessSelector Value="1" />
//      <AccessParameters>
//        <Structure Qty="4" >
//          <Structure Qty="4" >
//            <UInt16 Value="8" />
//            <!--0.0.1.0.0.255-->
//            <OctetString Value="0000010000FF" />
//            <Int8 Value="2" />
//            <UInt16 Value="0" />
//          </Structure>
//          <!--2018-06-07 17:16:27-->
//          <OctetString Value="07E206070411101BFFFFC480" />
//          <!--2018-06-07 17:30:27-->
//          <OctetString Value="07E2060704111E1BFFFFC480" />
//          <Array Qty="0" >
//          </Array>
//        </Structure>
//      </AccessParameters>
//    </AccessSelection>
//
// By entry:
//    <AccessSelection>
//      <AccessSelector Value="02" />
//      <AccessParameters>
//        <Structure Qty="04" >
//          <UInt32 Value="00000005" />
//          <UInt32 Value="00000005" />
//          <UInt16 Value="0001" />
//          <UInt16 Value="0013" />
//        </Structure>
//      </AccessParameters>
//    </AccessSelection>
//
void dlmsFormNoAccessSelection( AXDRStream* stream )
{
    axdrSetUint8( stream, 0 );  // access selection flag.
}

void dlmsFormProfileByRangeAccessSelection( AXDRStream* stream, uint32_t from, uint32_t to )
{
    axdrSetUint8( stream, 1 );  // access selection flag.
    axdrSetUint8( stream, 2 );  // by-range access selector.

    // TODO:
}

void dlmsFormProfileByEntryAccessSelection( AXDRStream* stream, uint32_t from, uint32_t to )
{
    axdrSetUint8( stream, 1 );  // access selection flag.
    axdrSetUint8( stream, 1 );  // by-entry access selector.

    // TODO:
}


void dlmsParseAccessSelection( AXDRStream* stream, bool* accessSelection, uint8_t* accessSelector )
{
    uint8_t selectionFlag   = 0;
    axdrGetUint8( stream, &selectionFlag );
    if(selectionFlag == 0)
    {
        *accessSelection    = false;
    }
    else
    {
        *accessSelection    = true;
        axdrGetUint8( stream, accessSelector );
    }
}



