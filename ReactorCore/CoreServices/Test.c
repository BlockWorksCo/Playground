/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process.
 */

/*
 * device specifics, such as ioctl numbers and the
 * major device file.
 */
#include "CoreServices.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>      /* ioctl */
#include <ctype.h>




/*
 * Main - Call the ioctl functions
 */
int main(int argc, char* argv[])
{
    int file_desc, ret_val;
    char* msg = "Message passed by ioctl\n";

    file_desc = open(DEVICE_FILE_NAME, 0);

    if (file_desc < 0)
    {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }

    if(argc == 2)
    {
        int ret_val;
        ret_val = ioctl(file_desc, IOCTL_SET_MSG, atoi(argv[1]) );

        if (ret_val < 0)
        {
            printf("ioctl_set_msg failed:%d\n", ret_val);
            exit(-1);
        }
    }
    else
    {
        printf("no args.\n");
    }
    close(file_desc);
}