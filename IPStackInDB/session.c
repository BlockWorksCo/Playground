



#include "session.h"
#include "udp.h"
#include "PersistentQueue.h"
#include <unistd.h>
#include <pthread.h>

#include <string.h>
#include <stdio.h>

#include <wolfssl/ssl.h>
#include <wolfssl/wolfio.h>


// TODO: Place these in a session-context.
static struct WOLFSSL_CTX*  ctx         = NULL;
WOLFSSL*                    ssl         = NULL;

IPv6Address*    lastRxPacketSrc         = NULL;
IPv6Address*    lastRxPacketDst         = NULL;
uint16_t        lastRxPacketSrcPort     = 0;
uint16_t        lastRxPacketDstPort     = 0;

size_t          numberOfBytesInCurrentPacket    = 0;
size_t          currentPacketReadOffset         = 0;
uint8_t*        currentPacket                   = NULL;



int myCBIORecv(WOLFSSL *ssl, char *buf, int sz, void *ctx)
{
    printf("<enter %s>\n",__func__);

    // Read up to the specified amount from the current packet or
    // whatever we have.
    int numBytesToRead = numberOfBytesInCurrentPacket;
    if (numBytesToRead > sz) {
        numBytesToRead = sz;
    }

    // Read data from the currentPacket or let the callee know we
    // haven't got any data.
    if (numBytesToRead > 0) {
       
        // Take the bytes from the currentPacket...
        memcpy( buf, &currentPacket[currentPacketReadOffset], numBytesToRead );

        // ...then move the pointers onwards.
        numberOfBytesInCurrentPacket    -= numBytesToRead;
        currentPacketReadOffset         += numBytesToRead;

        // Discard packet if we've read it all
        if( numberOfBytesInCurrentPacket <= 0 ) {

            // clear it all down.
            free(currentPacket);
            numberOfBytesInCurrentPacket    = 0;
            currentPacketReadOffset         = 0;
            currentPacket                   = NULL;
        }
    }
    else {

        printf("need more data!\n");
        // Need more data.
        return WOLFSSL_CBIO_ERR_WANT_READ;
    }

    printf("<exit %s numBytesToRead=%d>\n",__func__,numBytesToRead);

    return numBytesToRead;
}

int myCBIOSend(WOLFSSL *ssl, char *buf, int sz, void *ctx)
{
    printf("<enter %s>\n",__func__);

    // send the packet back to the originator.
    encodeUDPFrame( lastRxPacketDst, lastRxPacketSrc, lastRxPacketDstPort, lastRxPacketSrcPort, buf, sz );

    printf("<exit %s>\n",__func__);

    return sz;
}




void sessionProcessUDPPacket(IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* packet, size_t numberOfBytes )
{
    printf("<enter %s>\n",__func__);

    // TODO: Put this in a session context.
    lastRxPacketSrc     = dst;
    lastRxPacketDst     = src;
    lastRxPacketSrcPort = dstPort;
    lastRxPacketDstPort = srcPort;
    
    if( currentPacket != NULL) {
        printf("got packet too quickly, dropping it.\n");
    }
    else {

        // Take a copy of the packet.
        currentPacket   = (uint8_t*)malloc(numberOfBytes);
        numberOfBytesInCurrentPacket    = numberOfBytes;
        currentPacketReadOffset         = 0;
        memcpy( &currentPacket[0], packet, numberOfBytes );
    }

    // Let WolfSSL process the packet via the I/O callbacks.
    int res = wolfSSL_read( ssl, packet, numberOfBytes );

    //
    //res = wolfSSL_write( ssl, packet, numberOfBytes);

#ifdef CONFIG_ECHO_SERVER
    // Process payload
    uint8_t string[128] = {0};
    memcpy( &string[0], packet, numberOfBytes );
    printf("[%s]\n", string);

    //
    // echo the packet back.
    //
    uint8_t     response[128]   = {0};
    sprintf(response,"[%s]",string);
    encodeUDPFrame( src, dst, srcPort, dstPort, response, strlen(response) );
#endif

    printf("<exit %s>\n",__func__);
}



void sessionInit()
{
    //
    wolfSSL_Init();

    //
    wolfSSL_Debugging_ON();

    /* Set ctx to DTLS 1.2 */
    if ((ctx = wolfSSL_CTX_new(wolfDTLSv1_2_server_method())) == NULL) {
        printf("wolfSSL_CTX_new error.\n");
    }

    char        caCertLoc[] = "ca-cert.pem";
    char        servCertLoc[] = "server-cert.pem";
    char        servKeyLoc[] = "server-key.pem";

     /* Set ctx to DTLS 1.2 */
     if ((ctx = wolfSSL_CTX_new(wolfDTLSv1_2_server_method())) == NULL) {
         printf("wolfSSL_CTX_new error.\n");
         return;
     }
     /* Load CA certificates */
     if (wolfSSL_CTX_load_verify_locations(ctx,caCertLoc,0) !=
             SSL_SUCCESS) {
         printf("Error loading %s, please check the file.\n", caCertLoc);
         return;
     }
     /* Load server certificates */
     if (wolfSSL_CTX_use_certificate_file(ctx, servCertLoc, SSL_FILETYPE_PEM) !=
             SSL_SUCCESS) {
         printf("Error loading %s, please check the file.\n", servCertLoc);
         return;
     }
     /* Load server Keys */
     if (wolfSSL_CTX_use_PrivateKey_file(ctx, servKeyLoc,
                 SSL_FILETYPE_PEM) != SSL_SUCCESS) {
         printf("Error loading %s, please check the file.\n", servKeyLoc);
         return;
     }


    /* Create the WOLFSSL Object */
    int         cont = 0;
    static int cleanup = 0;
    if ((ssl = wolfSSL_new(ctx)) == NULL) {
        printf("wolfSSL_new error.\n");
        cleanup = 1;
        cont = 1;
    }

    // setup IO plumbing
    wolfSSL_SSLSetIORecv(ssl, myCBIORecv);
    wolfSSL_SSLSetIOSend(ssl, myCBIOSend);
    wolfSSL_set_using_nonblock(ssl, 1);

    // Setup current context.
    wolfSSL_SetIOReadCtx(ssl, NULL);
    wolfSSL_SetIOWriteCtx(ssl, NULL);

    // turn on EC support
    wolfSSL_CTX_UseSupportedCurve(ctx, WOLFSSL_ECC_SECP256R1);
    // turn on trusted_ca_keys extension support
    //wolfSSL_CTX_UseTrustedCaKeys(ctx);

    // limit the list of supported suites
    wolfSSL_CTX_set_cipher_list(ctx, "ECDHE-ECDSA-AES128-CCM-8");
    wolfSSL_CTX_set_cipher_list(ctx, "ECDHE-ECDSA-AES128-CBC-SHA");

    //
    wolfSSL_set_verify(ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);

    //
    //int res = wolfSSL_negotiate(ssl);

    if( wolfSSL_is_init_finished(ssl) != 0 ) {
        printf("DTLS session negotiated.\n");
    }
}

