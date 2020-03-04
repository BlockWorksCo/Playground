
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
#include "PersistentQueue.h"

//#include <wolfssl/options.h>
#include <wolfssl/ssl.h>

/* buffer for reading from tun/tap interface, must be >= 1500 */
#define BUFSIZE 2000
#define CLIENT 0
#define SERVER 1
#define PORT 55555

/* some common lengths */
#define IP_HDR_LEN 20
#define ETH_HDR_LEN 14
#define ARP_PKT_LEN 28


void transmitIPv6Packet( uint8_t* data, size_t numberOfBytes )
{
    printf("<writing %zd bytes to tun>\n", numberOfBytes);
    cwrite(tun_fd, data, numberOfBytes);
}

void processIPv6Packet( uint8_t* data, size_t numberOfBytes )
{
    //
    // Decode an ipv6 frame.
    //
    decodeFrame( data, numberOfBytes );
}


int myCBIORecv(WOLFSSL *ssl, char *buf, int sz, void *ctx)
{
}

int myCBIOSend(WOLFSSL *ssl, char *buf, int sz, void *ctx)
{
}


int main(int argc, char* argv[])
{
    int option;
    int flags = IFF_TUN;
    char if_name[IFNAMSIZ] = "tun1";
    int maxfd;
    uint16_t nread, nwrite, plength;
    char buffer[BUFSIZE];

    //
    pqInit(0, BUFSIZE, 16, processIPv6Packet);
    pqInit(1, BUFSIZE, 16, transmitIPv6Packet);

    //
    wolfSSL_Init();

    /* Set ctx to DTLS 1.2 */
    WOLFSSL_CTX* ctx;
    if ((ctx = wolfSSL_CTX_new(wolfDTLSv1_2_server_method())) == NULL) {
        printf("wolfSSL_CTX_new error.\n");
        return 1;
    }

    char        caCertLoc[] = "../certs/ca-cert.pem";
    char        servCertLoc[] = "../certs/server-cert.pem";
    char        servKeyLoc[] = "../certs/server-key.pem";

    /* Load CA certificates */
    if (wolfSSL_CTX_load_verify_locations(ctx,caCertLoc,0) !=
            SSL_SUCCESS) {
        printf("Error loading %s, please check the file.\n", caCertLoc);
        return 1;
    }

    /* Load server certificates */
    if (wolfSSL_CTX_use_certificate_file(ctx, servCertLoc, SSL_FILETYPE_PEM) != 
                                                                 SSL_SUCCESS) {
        printf("Error loading %s, please check the file.\n", servCertLoc);
        return 1;
    }

    /* Load server Keys */
    if (wolfSSL_CTX_use_PrivateKey_file(ctx, servKeyLoc,
                SSL_FILETYPE_PEM) != SSL_SUCCESS) {
        printf("Error loading %s, please check the file.\n", servKeyLoc);
        return 1;
    }

    /* Create the WOLFSSL Object */
    WOLFSSL*      ssl = NULL;
    int         cont = 0;
    static int cleanup = 0;
    if ((ssl = wolfSSL_new(ctx)) == NULL) {
        printf("wolfSSL_new error.\n");
        cleanup = 1;
        cont = 1;
    }

    //
    wolfSSL_SetIORecv(ctx, myCBIORecv);
    wolfSSL_SetIOSend(ctx, myCBIOSend);

    wolfSSL_SetIOReadCtx(ssl, NULL);
    wolfSSL_SetIOWriteCtx(ssl, NULL);
    wolfSSL_set_using_nonblock(ssl, 1);


    // turn on EC support
    wolfSSL_CTX_UseSupportedCurve(ctx, WOLFSSL_ECC_SECP256R1);
    // turn on trusted_ca_keys extension support
    //wolfSSL_CTX_UseTrustedCaKeys(ctx);

    // limit the list of supported suites
    wolfSSL_CTX_set_cipher_list(ctx, "ECDHE-ECDSA-AES128-CCM-8");

    //
    wolfSSL_set_verify(ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);


    //
    int res = wolfSSL_negotiate(ssl);

    if( wolfSSL_is_init_finished(ssl) != 0 ) {
        printf("DTLS session negotiated.\n");
    }


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
            pqPut( 0, &buffer[0], nread );
        }
    }

    return(0);
}

