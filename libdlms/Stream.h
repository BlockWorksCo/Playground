


#ifndef __STREAM_H__
#define __STREAM_H__





typedef uint8_t*    AXDRStream;  

void axdrSetUint8(AXDRStream* stream, uint8_t value);
void axdrSetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements);
void axdrGetUint8(AXDRStream* stream, uint8_t* value);
void axdrGetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements);

#endif
