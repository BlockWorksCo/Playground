

#include "axdr.h"
#include "dlms.h"




//
// TODO: Make this do something useful.
//
void dlmsFormTimeFromUTC( Stream* stream, uint32_t utc )
{
    uint8_t timeString[]    = {0,1,2,3,4,5,6,7,8,9,10,11};
    //timeString[0]   = utc;
    axdrSetOctetString( stream, (void*)&timeString, sizeof(timeString) );
}

void dlmsParseTime( Stream* stream, uint32_t* utc )
{
    uint8_t     timeString[]    = {0,1,2,3,4,5,6,7,8,9,10,11};
    uint32_t    stringLength    = 0;
    axdrGetOctetString( stream, (void*)timeString,sizeof(timeString), &stringLength );
    *utc    = timeString[0];
}


