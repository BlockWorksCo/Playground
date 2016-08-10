


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>




//
//
//
int main()
{
    printf("Aura DeviceSide demo.\n");
    int     fd  = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY );
    if(fd == -1)
    {
        printf("<Couldn't open UART>\n");
        exit(-1);
    }
    else
    {
        printf("<Opened UART>\n");

        while(true)
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);
            struct timeval timeout = { 1, 0 }; /* 10 seconds */

            int ret = select(fd+1, &fds, NULL, NULL, &timeout);
            if (ret ==1)
            {
                uint8_t         rxData  = 0;
                read( fd, &rxData, 1 );
                printf("%c", rxData);
            }
            else if(ret == 0)
            {
                perror("timeout error ");
            }
            else if (ret ==-1)
            {
                perror("some other error");
            }

        }

    }

}
