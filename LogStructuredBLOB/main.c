

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


/////////////////////////////////////////////////////////////////////

#define FLASH_FILENAME      ("flash.bin")
#define FLASH_SECTOR_SIZE   (128)


void flashEraseSector( uint32_t sectorId )
{
    FILE*   pFile   = NULL;

    pFile   = fopen( FLASH_FILENAME, "wb+" );
    if( pFile != NULL )
    {
        uint8_t temp[FLASH_SECTOR_SIZE];
        memset( &temp[0], 0xff, sizeof(temp) );
        fwrite( &temp[0], 1, sizeof(temp), pFile );
    }
}


void flashWriteSector( uint32_t sectorId, uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
    FILE*   pFile   = NULL;

    pFile   = fopen( FLASH_FILENAME, "wb+" );
    if( pFile != NULL )
    {
        fseek( pFile, (FLASH_SECTOR_SIZE*sectorId)+offset, SEEK_SET );
        fwrite( &bytes[0], 1, numberOfBytes, pFile );
    }
}


void flashReadSector( uint32_t sectorId, uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
    FILE*   pFile   = NULL;

    pFile   = fopen( FLASH_FILENAME, "wb+" );
    if( pFile != NULL )
    {
        fseek( pFile, (FLASH_SECTOR_SIZE*sectorId)+offset, SEEK_SET );
        fread( &bytes[0], 1, numberOfBytes, pFile );
    }
}


/////////////////////////////////////////////////////////////////////


void lsbWriteSpan( uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
}


void lsbReadSpan( uint32_t offset, uint8_t* bytes, uint32_t numberOfBytes )
{
}



/////////////////////////////////////////////////////////////////////

int main()
{
    flashEraseSector( 10 );

    uint8_t     stuff1[]   = "Hello World!";
    flashWriteSector( 10, 10, &stuff1[0], sizeof(stuff1) );

    uint8_t     stuff2[1024]    = {0};
    flashReadSector( 10, 10, &stuff2[0], sizeof(stuff1) );

    printf("[%s]\n", stuff2);
}


