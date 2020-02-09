
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

/* buffer for reading from tun/tap interface, must be >= 1500 */
#define BUFSIZE 2000
#define CLIENT 0
#define SERVER 1
#define PORT 55555

/* some common lengths */
#define IP_HDR_LEN 20
#define ETH_HDR_LEN 14
#define ARP_PKT_LEN 28

int debug;
char* progname;

/**************************************************************************
 * tun_alloc: allocates or reconnects to a tun/tap device. The caller     *
 *            needs to reserve enough space in *dev.                      *
 **************************************************************************/
int tun_alloc(char* dev, int flags)
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
    {
        perror("Opening /dev/net/tun");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;

    if (*dev)
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( (err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0 )
    {
        perror("ioctl(TUNSETIFF)");
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

/**************************************************************************
 * do_debug: prints debugging stuff (doh!)                                *
 **************************************************************************/
void do_debug(char* msg, ...)
{
    va_list argp;

    if(debug)
    {
        va_start(argp, msg);
        vfprintf(stderr, msg, argp);
        va_end(argp);
    }
}

/**************************************************************************
 * my_err: prints custom error messages on stderr.                        *
 **************************************************************************/
void my_err(char* msg, ...)
{
    va_list argp;
    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);
}

/**************************************************************************
 * usage: prints usage and exits.                                         *
 **************************************************************************/
void usage(void)
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "%s -i <ifacename> [-s|-c <serverIP>] [-p <port>] [-u|-a] [-d]\n", progname);
    fprintf(stderr, "%s -h\n", progname);
    fprintf(stderr, "\n");
    fprintf(stderr, "-i <ifacename>: Name of interface to use (mandatory)\n");
    fprintf(stderr, "-s|-c <serverIP>: run in server mode (-s), or specify server address (-c <serverIP>) (mandatory)\n");
    fprintf(stderr, "-p <port>: port to listen on (if run in server mode) or to connect to (in client mode), default 55555\n");
    fprintf(stderr, "-u|-a: use TUN (-u, default) or TAP (-a)\n");
    fprintf(stderr, "-d: outputs debug information while running\n");
    fprintf(stderr, "-h: prints this help text\n");
    exit(1);
}


int main(int argc, char* argv[])
{
    int option;
    int flags = IFF_TUN;
    char if_name[IFNAMSIZ] = "";
    int header_len = IP_HDR_LEN;
    int maxfd;
    uint16_t nread, nwrite, plength;
    //  uint16_t total_len, ethertype;
    char buffer[BUFSIZE];
    struct sockaddr_in local, remote;
    char remote_ip[16] = "";
    unsigned short int port = PORT;
    int sock_fd, optval = 1;
    socklen_t remotelen;
    int cliserv = -1;    /* must be specified on cmd line */
    unsigned long int tap2net = 0, net2tap = 0;
    progname = argv[0];

    /*
        uint8_t test0[]    = {0x60, 0x00, 0x00, 0x00, 0x00, 0x34, 0x11, 0x01, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xAB, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x60, 0x26, 0x92, 0x26, 0x92, 0x00, 0x0C, 0x7E, 0xD5, 0x12, 0x34, 0x56, 0x78};
        decodeFrame( &test0[0], sizeof(test0) );
    */

    /* Check command line options */
    while((option = getopt(argc, argv, "i:sc:p:uahd")) > 0)
    {
        switch(option)
        {
        case 'd':
            debug = 1;
            break;

        case 'h':
            usage();
            break;

        case 'i':
            strncpy(if_name, optarg, IFNAMSIZ - 1);
            break;

        case 's':
            cliserv = SERVER;
            break;

        case 'c':
            cliserv = CLIENT;
            strncpy(remote_ip, optarg, 15);
            break;

        case 'p':
            port = atoi(optarg);
            break;

        case 'u':
            flags = IFF_TUN;
            break;

        case 'a':
            flags = IFF_TAP;
            header_len = ETH_HDR_LEN;
            break;

        default:
            my_err("Unknown option %c\n", option);
            usage();
        }
    }

    argv += optind;
    argc -= optind;

    if(argc > 0)
    {
        my_err("Too many options!\n");
        usage();
    }

    if(*if_name == '\0')
    {
        my_err("Must specify interface name!\n");
        usage();
    }

    else if(cliserv < 0)
    {
        my_err("Must specify client or server mode!\n");
        usage();
    }

    else if((cliserv == CLIENT) && (*remote_ip == '\0'))
    {
        my_err("Must specify server address!\n");
        usage();
    }

    /* initialize tun/tap interface */
    if ( (tun_fd = tun_alloc(if_name, flags | IFF_NO_PI)) < 0 )
    {
        my_err("Error connecting to tun/tap interface %s!\n", if_name);
        exit(1);
    }

    do_debug("Successfully connected to interface %s\n", if_name);

    if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket()");
        exit(1);
    }

    /* use select() to handle two descriptors at once */
    maxfd = tun_fd;
    printf("Entering main loop.\n");

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
            tap2net++;
            do_debug("TAP2NET %lu: Read %d bytes from the tap interface\n", tap2net, nread);
            /* write length + packet */
            plength = htons(nread);
            //nwrite = cwrite(net_fd, (char*)&plength, sizeof(plength));
            //nwrite = cwrite(net_fd, buffer, nread);
            do_debug("TAP2NET %lu: Written %d bytes to the network\n", tap2net, nwrite);
            //
            // Decode an ethernet frame.
            //
            decodeFrame( &buffer[0], nread );
        }
    }

    return(0);
}

