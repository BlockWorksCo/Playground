
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

#include "layer1.h"
#include "tun.h"
#include "ipv6.h"
#include "udpQueue.h"

/* buffer for reading from tun/tap interface, must be >= 1500 */
#define BUFSIZE 2000
#define CLIENT 0
#define SERVER 1
#define PORT 55555

/* some common lengths */
#define IP_HDR_LEN 20
#define ETH_HDR_LEN 14
#define ARP_PKT_LEN 28




int main(int argc, char* argv[])
{
    int option;
    int flags = IFF_TUN;
    char if_name[IFNAMSIZ] = "tun1";
    int maxfd;
    uint16_t nread, nwrite, plength;
    char buffer[BUFSIZE];

    //
    udpQueueInit(0);
    udpQueueInit(1);

    /* initialize tun/tap interface */
    if ( (tun_fd = tun_alloc(if_name, flags | IFF_NO_PI)) < 0 )
    {
        printf("Error connecting to tun/tap interface %s!\n", if_name);
        exit(1);
    }

    printf("Successfully connected to interface %s\n", if_name);

    /* use select() to handle two descriptors at once */
    maxfd = tun_fd;
    printf("Entering main loop.\n");

    //
    while(1)
    {
        int ret;
        fd_set rd_set;
        FD_ZERO(&rd_set);
        FD_SET(tun_fd, &rd_set);
        ret = select(maxfd + 1, &rd_set, NULL, NULL, NULL);

        if (ret < 0 && errno == EINTR)
        {
            continue;
        }

        if (ret < 0)
        {
            perror("select()");
            exit(1);
        }

        if(FD_ISSET(tun_fd, &rd_set))
        {
            /* data from tun/tap: just read it and write it to the network */
            nread = cread(tun_fd, buffer, BUFSIZE);
            printf("Read %d bytes from the tap interface\n", nread);
            /* write length + packet */
            plength = htons(nread);
            printf("Written %d bytes to the network\n", nwrite);
            //
            // Decode an ethernet frame.
            //
            decodeFrame( &buffer[0], nread );
        }
    }

    return(0);
}

