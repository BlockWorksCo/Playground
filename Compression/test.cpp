


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "smallz4.h"


char    inputData[]             = "Hello World Hello World Hello World.";
char    outputData[128]         = {0};
char    decompData[128]         = {0};

char*   input                   = NULL;
size_t  inputBytesLeft          = 0;
char*   output                  = NULL;
size_t  outputSpaceAvailable    = 0;


// read one byte from input, see getByteFromIn()  for a basic implementation
typedef unsigned char (*decompGET_BYTE)  ();
// write several bytes,      see sendBytesToOut() for a basic implementation
typedef void          (*decompSEND_BYTES)(const void*, size_t);



unsigned char getByte()
{
    if( inputBytesLeft > 0 ) {
        unsigned char returnValue  = *input;
        input++;
        inputBytesLeft--;
        return returnValue;
    }

    return 0;
}

size_t getBytes( void* data, size_t numBytes )
{
    if( inputBytesLeft > 0 ) {
        *((char*)data)   = *input;
        input++;
        inputBytesLeft--;
        return 1;
    }

    return 0;
}

void putBytes(const void* data, size_t numBytes )
{
    char*   cData   = (char*)data;
    while( outputSpaceAvailable > 0 && numBytes > 0 ) {
        *output = *((char*)cData);
        output++;
        outputSpaceAvailable--;
        numBytes--;
        cData++;
    }
}





#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

/// error handler
void error(const char* msg)
{
  // smaller static binary than fprintf(stderr, "ERROR: %s\n", msg);
  fputs("ERROR: ", stderr);
  fputs(msg,       stderr);
  fputc('\n',      stderr);
  exit(1);
}


// ==================== I/O INTERFACE ====================


/// read a single byte (with simple buffering)
static unsigned char decompressorBytesIn()
{
}

/// write a block of bytes
static void decompressorBytesOut(const unsigned char* data, unsigned int numBytes)
{
}


// ==================== LZ4 DECOMPRESSOR ====================

/// decompress everything in input stream (accessed via getByte) and write to output stream (via sendBytes)
void unlz4(decompGET_BYTE getByte, decompSEND_BYTES sendBytes, const char* dictionary)
{
  // signature
  unsigned char signature1 = getByte();
  unsigned char signature2 = getByte();
  unsigned char signature3 = getByte();
  unsigned char signature4 = getByte();
  unsigned int  signature  = (signature4 << 24) | (signature3 << 16) | (signature2 << 8) | signature1;
  unsigned char isModern   = (signature == 0x184D2204);
  unsigned char isLegacy   = (signature == 0x184C2102);
  if (!isModern && !isLegacy)
    error("invalid signature");

  unsigned char hasBlockChecksum   = FALSE;
  unsigned char hasContentSize     = FALSE;
  unsigned char hasContentChecksum = FALSE;
  unsigned char hasDictionaryID    = FALSE;
  if (isModern)
  {
    // flags
    unsigned char flags = getByte();
    hasBlockChecksum   = flags & 16;
    hasContentSize     = flags &  8;
    hasContentChecksum = flags &  4;
    hasDictionaryID    = flags &  1;

    // only version 1 file format
    unsigned char version = flags >> 6;
    if (version != 1)
      error("only LZ4 file format version 1 supported");

    // ignore blocksize
    getByte();

    if (hasContentSize)
    {
      // ignore, skip 8 bytes
      getByte(); getByte(); getByte(); getByte();
      getByte(); getByte(); getByte(); getByte();
    }
    if (hasDictionaryID)
    {
      // ignore, skip 4 bytes
      getByte(); getByte(); getByte(); getByte();
    }

    // ignore header checksum (xxhash32 of everything up this point & 0xFF)
    getByte();
  }

  // don't lower this value, backreferences can be 64kb far away
#define HISTORY_SIZE 64*1024
  // contains the latest decoded data
  unsigned char history[HISTORY_SIZE];
  // next free position in history[]
  unsigned int  pos = 0;

  // dictionary compression is a recently introduced feature, just move its contents to the buffer
  if (dictionary != NULL)
  {
    // open dictionary
    FILE* dict = fopen(dictionary, "rb");
    if (!dict)
      error("cannot open dictionary");

    // get dictionary's filesize
    fseek(dict, 0, SEEK_END);
    long dictSize = ftell(dict);
    // only the last 64k are relevant
    long relevant = dictSize < 65536 ? 0 : dictSize - 65536;
    fseek(dict, relevant, SEEK_SET);
    if (dictSize > 65536)
      dictSize = 65536;
    // read it and store it at the end of the buffer
    fread(history + HISTORY_SIZE - dictSize, 1, dictSize, dict);
    fclose(dict);
  }

  // parse all blocks until blockSize == 0
  while (1)
  {
    // block size
    unsigned int blockSize = getByte();
    blockSize |= (unsigned int)getByte() <<  8;
    blockSize |= (unsigned int)getByte() << 16;
    blockSize |= (unsigned int)getByte() << 24;

    // highest bit set ?
    unsigned char isCompressed = isLegacy || (blockSize & 0x80000000) == 0;
    if (isModern)
      blockSize &= 0x7FFFFFFF;

    // stop after last block
    if (blockSize == 0)
      break;

    if (isCompressed)
    {
      // decompress block
      unsigned int blockOffset = 0;
      unsigned int numWritten  = 0;
      while (blockOffset < blockSize)
      {
        // get a token
        unsigned char token = getByte();
        blockOffset++;

        // determine number of literals
        unsigned int numLiterals = token >> 4;
        if (numLiterals == 15)
        {
          // number of literals length encoded in more than 1 byte
          unsigned char current;
          do
          {
            current = getByte();
            numLiterals += current;
            blockOffset++;
          } while (current == 255);
        }

        blockOffset += numLiterals;

        // copy all those literals
        if (pos + numLiterals < HISTORY_SIZE)
        {
          // fast loop
          while (numLiterals-- > 0)
            history[pos++] = getByte();
        }
        else
        {
          // slow loop
          while (numLiterals-- > 0)
          {
            history[pos++] = getByte();

            // flush output buffer
            if (pos == HISTORY_SIZE)
            {
              sendBytes(history, HISTORY_SIZE);
              numWritten += HISTORY_SIZE;
              pos = 0;
            }
          }
        }

        // last token has only literals
        if (blockOffset == blockSize)
          break;

        // match distance is encoded in two bytes (little endian)
        unsigned int delta = getByte();
        delta |= (unsigned int)getByte() << 8;
        // zero isn't allowed
        if (delta == 0)
          error("invalid offset");
        blockOffset += 2;

        // match length (always >= 4, therefore length is stored minus 4)
        unsigned int matchLength = 4 + (token & 0x0F);
        if (matchLength == 4 + 0x0F)
        {
          unsigned char current;
          do // match length encoded in more than 1 byte
          {
            current = getByte();
            matchLength += current;
            blockOffset++;
          } while (current == 255);
        }

        // copy match
        unsigned int referencePos = (pos >= delta) ? (pos - delta) : (HISTORY_SIZE + pos - delta);
        // start and end within the current 64k block ?
        if (pos + matchLength < HISTORY_SIZE && referencePos + matchLength < HISTORY_SIZE)
        {
          // read/write continuous block (no wrap-around at the end of history[])
          // fast copy
          if (pos >= referencePos + matchLength || referencePos >= pos + matchLength)
          {
            // non-overlapping
            memcpy(history + pos, history + referencePos, matchLength);
            pos += matchLength;
          }
          else
          {
            // overlapping, slower byte-wise copy
            while (matchLength-- > 0)
              history[pos++] = history[referencePos++];
          }
        }
        else
        {
          // either read or write wraps around at the end of history[]
          while (matchLength-- > 0)
          {
            // copy single byte
            history[pos++] = history[referencePos++];

            // cannot write anymore ? => wrap around
            if (pos == HISTORY_SIZE)
            {
              // flush output buffer
              sendBytes(history, HISTORY_SIZE);
              numWritten += HISTORY_SIZE;
              pos = 0;
            }
            // wrap-around of read location
            referencePos %= HISTORY_SIZE;
          }
        }
      }

      // all legacy blocks must be completely filled - except for the last one
      if (isLegacy && numWritten + pos < 8*1024*1024)
        break;
    }
    else
    {
      // copy uncompressed data and add to history, too (if next block is compressed and some matches refer to this block)
      while (blockSize-- > 0)
      {
        // copy a byte ...
        history[pos++] = getByte();
        // ... until buffer is full => send to output
        if (pos == HISTORY_SIZE)
        {
          sendBytes(history, HISTORY_SIZE);
          pos = 0;
        }
      }
    }

    if (hasBlockChecksum)
    {
      // ignore checksum, skip 4 bytes
      getByte(); getByte(); getByte(); getByte();
    }
  }

  if (hasContentChecksum)
  {
    // ignore checksum, skip 4 bytes
    getByte(); getByte(); getByte(); getByte();
  }

  // flush output buffer
  sendBytes(history, pos);
}








int main()
{
    // Compress
    input                   = &inputData[0];
    inputBytesLeft          = sizeof(inputData);
    output                  = &outputData[0];
    outputSpaceAvailable    = sizeof(outputData);

    smallz4::lz4( getBytes, putBytes );

    size_t  outputBytesUsed = sizeof(outputData) - outputSpaceAvailable;
    printf("outputBytesUsed=%zd\n",outputBytesUsed);
    for(int i=0; i<outputBytesUsed; i++) {
        const char    hex[] = "0123456789abcdef";
        char    t   = outputData[i];
        int     hi  = (t >> 4)&0xf;
        int     lo  = t &0xf;
        printf("0x%c%c, ", hex[hi], hex[lo]);
    }
    printf("\n");

    // Decompress
    input                   = &outputData[0];
    inputBytesLeft          = outputBytesUsed;
    output                  = &decompData[0];
    outputSpaceAvailable    = sizeof(decompData);

    unlz4( getByte, putBytes, NULL );

    outputBytesUsed = sizeof(decompData) - outputSpaceAvailable;
    printf("(decomp) outputBytesUsed=%zd\n",outputBytesUsed);
    for(int i=0; i<outputBytesUsed; i++) {
        const char    hex[] = "0123456789abcdef";
        char    t   = decompData[i];
        int     hi  = (t >> 4)&0xf;
        int     lo  = t &0xf;
        printf("0x%c%c, ", hex[hi], hex[lo]);
    }
    printf("\n");

    printf("[%s]\n", decompData);

}



