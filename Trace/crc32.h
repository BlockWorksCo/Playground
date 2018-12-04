

#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdint.h>

void crc32 ( const void * key, int len, uint32_t seed, uint32_t * out );


#endif

