
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
        perror("Writing data");
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


typedef uint8_t IPv6Address[16];
int tap_fd;

typedef struct
{
    IPv6Address src;
    IPv6Address dst;
    uint32_t    headerAndDataLength;
    uint8_t     zeroes[3];
    uint8_t     nextHeader;
    uint16_t    srcPort;
    uint16_t    dstPort;
    uint16_t    length;
    uint16_t    checksum;
    uint8_t     payload[1];

} __attribute__((__packed__)) UDPPsuedoHeader;


void dumpHex( uint8_t* bytes, size_t numberOfBytes )
{
    for(uint32_t i=0; i<numberOfBytes; i++) {
        printf("%02x ",bytes[i]);
    }
    printf("\n");
}


// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
uint16_t checksum (uint16_t *addr, int len)
{
  int count = len;
  register uint32_t sum = 0;
  uint16_t answer = 0;

  // Sum up 2-byte values until none or only one byte left.
  while (count > 1) {
    sum += *(addr++);
    count -= 2;
  }

  // Add left-over byte, if any.
  if (count > 0) {
    sum += *(uint8_t *) addr;
  }

  // Fold 32-bit sum into 16 bits; we lose information by doing this,
  // increasing the chances of a collision.
  // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }

  // Checksum is one's compliment of sum.
  answer = ~sum;

  return (answer);
}

uint16_t udpChecksum( IPv6Address src, IPv6Address dst, uint16_t srcPort, uint16_t dstPort, uint16_t payloadLen, uint8_t* payload )
{
  char buf[2048]    = {0};
  UDPPsuedoHeader*  header  = (UDPPsuedoHeader*)&buf[0];
  int chksumlen = 0;
  int i;

  // Copy source IP address into buf (128 bits)
  memcpy( &header->src, src, sizeof(IPv6Address) );
  chksumlen += sizeof (IPv6Address);

  // Copy destination IP address into buf (128 bits)
  memcpy( &header->dst, dst, sizeof(IPv6Address) );
  chksumlen += sizeof (IPv6Address);

  // Copy UDP length into buf (32 bits)
  header->headerAndDataLength   = 8 + payloadLen;
  chksumlen += sizeof (uint32_t);

  // Copy zero field to buf (24 bits)
  header->zeroes[0]  = 0;
  header->zeroes[1]  = 0;
  header->zeroes[2]  = 0;
  chksumlen += 3;

  // Copy next header field to buf (8 bits)
  header->nextHeader    = 0x11;
  chksumlen += sizeof (uint8_t);

  // Copy UDP source port to buf (16 bits)
  header->srcPort   = srcPort;
  chksumlen += sizeof (uint16_t);

  // Copy UDP destination port to buf (16 bits)
  header->dstPort   = dstPort;
  chksumlen += sizeof (uint16_t);

  // Copy UDP length again to buf (16 bits)
  header->length   = payloadLen;
  chksumlen += sizeof (uint16_t);

  // Copy UDP checksum to buf (16 bits)
  // Zero, since we don't know it yet
  header->checksum  = 0;
  chksumlen += sizeof (uint16_t);

  // Copy payload to buf
  memcpy( &header->payload[0], payload, payloadLen );
  chksumlen += payloadLen;

  // Pad to the next 16-bit boundary
  for (i=0; i<payloadLen%2; i++) {
    chksumlen++;
  }

  printf("src:");
  dumpHex( (uint8_t*)src, sizeof(IPv6Address) );
  printf("dst:");
  dumpHex( (uint8_t*)dst, sizeof(IPv6Address) );
  printf("checked frame:");
  dumpHex( (uint8_t*)buf, chksumlen );

  uint16_t checksumValue =  checksum((uint16_t *) buf, chksumlen);
  printf("checksumValue = %04x\n\n",checksumValue);

    return checksumValue;
}







void decodeFrame( uint8_t* frame, size_t numberOfBytes )
{
    printf("Decoding frame [%zd]\n",numberOfBytes);

    uint8_t frameType   = frame[0] & 0xe0;
    if(frameType == 0x60) {
        printf("IPv6\n");
    }
    if(frameType == 0x40) {
        printf("IPv4\n");
    }

    uint16_t*       ipv6PacketLength = (uint16_t*)&frame[4];
    uint8_t*        nextHeader          = (uint8_t*)&frame[6];
    IPv6Address*    src = (IPv6Address*)&frame[8];
    IPv6Address*    dst = (IPv6Address*)&frame[24];

    uint16_t*       srcPort = (uint16_t*)&frame[40];
    uint16_t*       dstPort = (uint16_t*)&frame[42];
    uint16_t*       udpPacketLength = (uint16_t*)&frame[44];
    uint16_t*       udpCheckSum = (uint16_t*)&frame[46];    // assume its zero/unused.

    uint8_t*        udpPayload  = &frame[48];

    printf("nextHeader=%02x srcPort=%d dstPort=%d udpLength=%d udpChecksum=%04x\n", *nextHeader, ntohs(*srcPort), ntohs(*dstPort), ntohs(*udpPacketLength), ntohs(*udpCheckSum) );

    // UDP packets sent to port 80.
    if((ntohs(*dstPort) == 9874) && (*nextHeader == 0x11)) {

        printf("incoming frame:");
        dumpHex( frame, numberOfBytes );

        // Integrity check.
        uint16_t checkValue = udpChecksum( *src, *dst, *srcPort, *dstPort, ntohs(*udpPacketLength)-8, udpPayload );

        // Process payload
        uint8_t string[128] = {0};
        memcpy( &string[0], udpPayload, ntohs(*udpPacketLength)-8 );
        printf("%02x [%s]\n", checkValue,string);

        //
        // echo the packet back.
        //
        uint8_t         packet[128];

        memcpy( &packet[0], frame, numberOfBytes );
        IPv6Address*    newSrc      = (IPv6Address*)&packet[8];
        IPv6Address*    newDst      = (IPv6Address*)&packet[24];
        uint16_t*       newSrcPort  = (uint16_t*)&packet[40];
        uint16_t*       newDstPort  = (uint16_t*)&packet[42];
        uint16_t*       newUDPCheckSum = (uint16_t*)&packet[46];

        memcpy( newSrc, dst, sizeof(IPv6Address) );
        memcpy( newDst, src, sizeof(IPv6Address) );
        *newSrcPort     = *dstPort; 
        *newDstPort     = *srcPort; 
        *newUDPCheckSum = udpChecksum( *newSrc, *newDst, *newSrcPort, *newDstPort, numberOfBytes, &packet[0] );

        // transmit the packet.
        uint16_t nwrite = cwrite(tap_fd, &packet[0], numberOfBytes);
        printf("replied...\n");
    }
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



    uint8_t test0[]    = {0x60, 0x00, 0x00, 0x00, 0x00, 0x34, 0x11, 0x01, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xAB, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x60, 0x26, 0x92, 0x26, 0x92, 0x00, 0x0C, 0x7E, 0xD5, 0x12, 0x34, 0x56, 0x78};
    decodeFrame( &test0[0], sizeof(test0) );


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
    if ( (tap_fd = tun_alloc(if_name, flags | IFF_NO_PI)) < 0 )
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
    maxfd = tap_fd;

    printf("Entering main loop.\n");
    while(1)
    {
        int ret;
        fd_set rd_set;
        FD_ZERO(&rd_set);
        FD_SET(tap_fd, &rd_set);
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

        if(FD_ISSET(tap_fd, &rd_set))
        {
            /* data from tun/tap: just read it and write it to the network */
            nread = cread(tap_fd, buffer, BUFSIZE);
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

#if 0
        if(FD_ISSET(net_fd, &rd_set))
        {
            /* data from the network: read it, and write it to the tun/tap interface.
             * We need to read the length first, and then the packet */
            /* Read length */
            nread = read_n(net_fd, (char*)&plength, sizeof(plength));

            if(nread == 0)
            {
                /* ctrl-c at the other end */
                break;
            }

            net2tap++;
            /* read packet */
            nread = read_n(net_fd, buffer, ntohs(plength));
            do_debug("NET2TAP %lu: Read %d bytes from the network\n", net2tap, nread);
            /* now buffer[] contains a full packet or frame, write it into the tun/tap interface */
            nwrite = cwrite(tap_fd, buffer, nread);
            do_debug("NET2TAP %lu: Written %d bytes to the tap interface\n", net2tap, nwrite);
        }
#endif
    }

    return(0);
}

