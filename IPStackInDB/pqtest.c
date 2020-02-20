
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>


#include "PersistentQueue.h"

#define BUFSIZE 2000


void processIPv6Packet( uint8_t* data, size_t numberOfBytes )
{
    //
    // Decode an ipv6 frame.
    //
    char    buffer[BUFSIZE] = {0};
    memcpy( &buffer[0], data, numberOfBytes);
    printf("%zdB [%s]\n", numberOfBytes, buffer);
}


int main(int argc, char* argv[])
{
    //
    pqInit(0, BUFSIZE, 16, processIPv6Packet);

    //
    pqPut( 0, "Hello World1", 13 );
    pqPut( 0, "Hello World2", 13 );
    pqPut( 0, "Hello World3", 13 );
    pqPut( 0, "Hello World4", 13 );
    pqPut( 0, "Hello World5", 13 );
    pqPut( 0, "Hello World6", 13 );
    pqPut( 0, "Hello World7", 13 );
    pqPut( 0, "Hello World8", 13 );

    sleep(1);

    return(0);
}

