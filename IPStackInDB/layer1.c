
#include "layer1.h"
#include "tun.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>




/**************************************************************************
 * cread: read routine that checks for errors and exits if an error is    *
 *        returned.                                                       *
 **************************************************************************/
int cread(int fd, char* buf, int n)
{
    int nread;

    if((nread = read(fd, buf, n)) < 0)
    {
        perror("Reading data");
        exit(1);
    }

    return nread;
}


/**************************************************************************
 * cwrite: write routine that checks for errors and exits if an error is  *
 *         returned.                                                      *
 **************************************************************************/
int cwrite(int fd, char* buf, int n)
{
    int nwrite;

    if((nwrite = write(fd, buf, n)) < 0)
    {
        printf("error Writing data");
        exit(1);
    }

    return nwrite;
}

/**************************************************************************
 * read_n: ensures we read exactly n bytes, and puts those into "buf".    *
 *         (unless EOF, of course)                                        *
 **************************************************************************/
int read_n(int fd, char* buf, int n)
{
    int nread, left = n;

    while(left > 0)
    {
        if ((nread = cread(fd, buf, left)) == 0)
        {
            return 0 ;
        }

        else
        {
            left -= nread;
            buf += nread;
        }
    }

    return n;
}




