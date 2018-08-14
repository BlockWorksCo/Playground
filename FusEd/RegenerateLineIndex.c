

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>




int main( int argc, char* argv[] )
{
    int     inFd  = -1;
    char*   inFileName  = argv[1];

    inFd  = open( inFileName, O_RDONLY );
    if(inFd != -1)
    {
        char*   outFileName = argv[2];
        int     outFd       = -1;
        outFd  = open( outFileName, O_RDWR|O_CREAT|O_TRUNC, 0666 );
        if(outFd != -1)
        {
            uint8_t     buffer[1024]    = {0};
            uint32_t    offset          = 0;
            ssize_t     bytesRead       = 0;
            uint32_t    lineCount       = 0;
            do
            {
                bytesRead   = read( inFd, buffer, sizeof(buffer));
                if(bytesRead > 0)
                {
                    for(uint32_t i=0; i<bytesRead; i++)
                    {
                        if( buffer[i] == '\n' )
                        {
                            uint32_t    temp    = offset+i;
                            write( outFd, &temp, sizeof(temp) );
                            lineCount++;
                            //printf("%08x\n",offset+i);
                        }
                    }
                    offset  += bytesRead;
                }

            } while(bytesRead > 0);

            printf("%d lines\n",lineCount);
        }
        else
        {
            printf("couldnt open out file.\n");
        }

        return 0;
    }
    else 
    {
        printf("failed.\n");
        return -1;
    }
}

