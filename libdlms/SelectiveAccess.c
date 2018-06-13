

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
//
// c001 81 0007 0100630100ff 02 01 01 0204 0204 120008 09060000010000ff 0f02 120000 090c07e2060704110e11ffffc480 090c07e2060704111c11ffffc480 0100 
// C001 81 0800 0000010000FF 02 01 01 0204 0204 120800 090c0000010000FF ff02 120000 090C000102030405060708090A0B 090C000102030405060708090A0B 0100
//
// <GetRequest>
//   <GetRequestNormal>
//     <InvokeIdAndPriority Value="81" />
//     <AttributeDescriptor>
//       <!--PROFILE_GENERIC-->
//       <ClassId Value="0007" />
//       <!--1.0.99.1.0.255-->
//       <InstanceId Value="0100630100FF" />
//       <AttributeId Value="02" />
//     </AttributeDescriptor>
//     <AccessSelection>
//       <AccessSelector Value="01" />
//       <AccessParameters>
//         <Structure Qty="04" >
//           <Structure Qty="04" >
//             <UInt16 Value="0008" />
//             <!--0.0.1.0.0.255-->
//             <OctetString Value="0000010000FF" />
//             <Int8 Value="02" />
//             <UInt16 Value="0000" />
//           </Structure>
//           <!--2018-06-07 17:14:17-->
//           <OctetString Value="07E2060704110E11FFFFC480" />
//           <!--2018-06-07 17:28:17-->
//           <OctetString Value="07E2060704111C11FFFFC480" />
//           <Array Qty="00" >
//           </Array>
//         </Structure>
//       </AccessParameters>
//     </AccessSelection>
//   </GetRequestNormal>
// </GetRequest>
// 
//
void dlmsFormNoAccessSelection( Stream* stream )
{
    streamSetUint8( stream, 0 );  // access selection flag.
}

void dlmsFormByTimeRangeAccessSelection( Stream* stream, uint32_t from, uint32_t to )
{
    streamSetUint8( stream, 1 );  // access selection flag.
    streamSetUint8( stream, 1 );  // by-range access selector.

    axdrSetStruct( stream, 4 ); // structure of 4 fields as the first field of 4 within a structure.
    axdrSetStruct( stream, 4 );

    OBISCode    timeOBIS    = {0,0,1,0,0,255};

    axdrSetUint16( stream, TimeClass );             
    axdrSetOctetString( stream, (void*)&timeOBIS,sizeof(OBISCode) );          
    axdrSetUint8( stream, 2 );   // Fix naming for this.

    axdrSetUint16( stream, 0 ); // data index (0=whole attribute).

    dlmsFormTimeFromUTC( stream, from );
    dlmsFormTimeFromUTC( stream, to );

    axdrSetArray( stream, 0 );  // Unused 4th field is empty array of columns.
}

void dlmsFormByEntryAccessSelection( Stream* stream, uint32_t from, uint32_t to )
{
    // 01 02 0204 0600000005 0600000005 120001 120013 

    streamSetUint8( stream, 1 );  // access selection flag.
    streamSetUint8( stream, 2 );  // by-entry access selector.

    axdrSetStruct( stream, 4 );

    axdrSetUint32( stream, from );
    axdrSetUint32( stream, to );

    axdrSetUint16( stream, 0 ); // from-selected-value (0=all columns).
    axdrSetUint16( stream, 0 ); // to-selected-value (0=all columns).
}



void dlmsParseAccessSelection( Stream* stream, bool* accessSelection, uint8_t* accessSelector )
{
    uint8_t selectionFlag   = 0;
    streamGetUint8( stream, &selectionFlag );
    if(selectionFlag == 0)
    {
        *accessSelection    = false;
    }
    else
    {
        *accessSelection    = true;
        streamGetUint8( stream, accessSelector );
    }
}


void dlmsParseByTimeRangeAccessSelection( Stream* stream, uint32_t* from, uint32_t* to )
{
    uint32_t     numberOfFields;

    axdrGetStruct( stream, &numberOfFields ); // structure of 4 fields as the first field of 4 within a structure.
    assert( numberOfFields == 4);

    axdrGetStruct( stream, &numberOfFields ); // structure of 4 fields as the first field of 4 within a structure.
    assert( numberOfFields == 4);

    OBISCode    timeOBIS    = {0,0,1,0,0,255};

    uint16_t    ic;
    axdrGetUint16( stream, &ic );             
    assert( ic == TimeClass );

    uint32_t    stringLength    = 0;
    axdrGetOctetString( stream, (void*)&timeOBIS,sizeof(OBISCode),&stringLength );          
    assert( stringLength == sizeof(timeOBIS) );

    uint8_t     attrId;
    axdrGetUint8( stream, &attrId );   // Fix naming for this.
    assert( attrId == 2 );

    uint16_t    dataIndex;
    axdrGetUint16( stream, &dataIndex ); // data index (0=whole attribute).
    assert( dataIndex == 0 );

    dlmsParseTime( stream, from );
    dlmsParseTime( stream, to );

    axdrGetArray( stream, &numberOfFields );  // Unused 4th field is empty array of columns.
    assert( numberOfFields == 0 );
}

void dlmsParseByEntryAccessSelection( Stream* stream, uint32_t* from, uint32_t* to )
{
    uint32_t     numberOfFields;

    axdrGetStruct( stream, &numberOfFields ); // structure of 4 fields as the first field of 4 within a structure.
    assert( numberOfFields == 4);

    axdrGetUint32( stream, from );             
    axdrGetUint32( stream, to );

    uint16_t    fromSelectedValue;
    axdrGetUint16( stream, &fromSelectedValue );
    assert( fromSelectedValue == 0 );

    uint16_t    toSelectedValue;
    axdrGetUint16( stream, &toSelectedValue );
    assert( toSelectedValue == 0 );
}




