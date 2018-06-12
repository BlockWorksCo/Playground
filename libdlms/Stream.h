


#ifndef __STREAM_H__
#define __STREAM_H__





typedef uint8_t*    Stream;  

void streamSetUint8(Stream* stream, uint8_t value);
void streamSetUint8Array(Stream* stream, uint8_t* values, uint32_t numberOfElements);
void streamGetUint8(Stream* stream, uint8_t* value);
void streamGetUint8Array(Stream* stream, uint8_t* values, uint32_t numberOfElements);

#endif
