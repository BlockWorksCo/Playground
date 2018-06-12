


#ifndef __STREAM_H__
#define __STREAM_H__





typedef uint8_t*    AXDRStream;  

void streamSetUint8(AXDRStream* stream, uint8_t value);
void streamSetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements);
void streamGetUint8(AXDRStream* stream, uint8_t* value);
void streamGetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements);

#endif
