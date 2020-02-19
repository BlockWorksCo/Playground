



#include "session.h"
#include "udp.h"
#include "PersistentQueue.h"
#include <unistd.h>
#include <pthread.h>

#include <string.h>
#include <stdio.h>

#include "wolfssl_config.h"
#include "wolfssl/ssl.h"
#include "wolfssl/error-ssl.h"
#include "wolfssl/wolfcrypt/asn.h"
#include "wolfssl/wolfcrypt/logging.h"


static struct WOLFSSL_CTX* s_ctx;

bool sessionExists  = false;


void setupSession()
{
    struct WOLFSSL* ssl = wolfSSL_new(s_ctx);
}


void sessionProcessUDPPacket(IPv6Address* src, IPv6Address* dst, uint16_t srcPort, uint16_t dstPort, uint8_t* packet, size_t numberOfBytes )
{
    //
    if(sessionExists == false) {
        setupSession();
        sessionExists   = true;
    }

    // Push the packet into the queue.
    //pqPut( 0, src, packet, numberOfBytes );

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
}



void sessionInit()
{
}

