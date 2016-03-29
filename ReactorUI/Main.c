//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, November 2015.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>

#include "libwebsockets.h"




//
//
//
int callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
    return 0;
}


uint32_t    signals[100][16];



void GenerateSignalData()
{
    for(uint32_t signal=0; signal<16; signal++)
    {
        for(uint32_t i=0; i<100; i++)
        {
            signals[i][signal]  = rand() % 2;
        }
    }
}



//
//
//
int ReactorUIProtocolCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
    //
    //
    //
    GenerateSignalData();

    //
    //
    //
    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
        printf("connection established\n");
        break;

    case LWS_CALLBACK_RECEIVE:   // the funny part
        {
            printf("received request\n");

            //char   buf[1024*2];
            char*   buf         = (char*)malloc(1024*32);
            char*   payload     = &buf[0];
            //char*   payload     = &buf[LWS_PRE];
            //strcpy(buf, "{ \"data\":[[11,22,33,44],[21,22,23,24],[31,32,33,34],[41,42,43,44]] }");

            strcpy(payload, "{ \"data\":[");
            for(uint32_t signal=0; signal<16; signal++)
            {
                //
                //
                //
                char    signalJSONData[1024*32]    = {0};
                strcpy(&signalJSONData[0], "\t[");
                for(uint32_t i=0; i<100; i++)
                {
                    char    temp[16]    = {0};
                    sprintf( &temp[0], "{\"x\":%d, \"y\":%d}", i*1, signals[i][signal] );

                    if( i > 0 )
                    {
                        strcat( &signalJSONData[0], ",");
                    }
                    strcat( &signalJSONData[0], temp);
                }
                strcat(&signalJSONData[0], "]");

                //printf("%d:%s\n", signal, signalJSONData);

                //
                //
                //
                if( signal > 0 )
                {
                    strcat( payload, ", \n" );
                }
                strcat( payload, &signalJSONData[0] );
            }
            strcat(payload, "] }");

            printf("\n\n(%d)    %s\n", (uint32_t)strlen(payload), payload);

            //
            //
            //
            lws_write(wsi, buf, strlen(buf), LWS_WRITE_TEXT);

            //free(buf);

            break;
        }

    default:
        break;
    }

    return 0;
}






/* list of supported protocols and callbacks */
static struct lws_protocols protocols[] =
{
    /* first protocol must always be HTTP handler */
    {
        "http-only",   // name
        callback_http, // callback
        0              // per_session_data_size
    },
    {
        "ReactorUI", // protocol name - very important!
        ReactorUIProtocolCallback,   // callback
        0                          // we don't use any per session data

    },
    {
        NULL, NULL, 0   /* End of list */
    }
};






int main(int argc, char** argv)
{
    //
    //
    //
    struct lws_context_creation_info   info    = {0};
    info.port           = 9000;
    info.iface          = NULL;
    info.protocols      = protocols;

    //
    // create libwebsocket context representing this server
    //
    struct lws_context* context = lws_create_context( &info );
    if (context == NULL)
    {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }

    printf("Starting ReactorUI server...\n");

    //
    // Forever....
    //
    while (1)
    {
        lws_service(context, 50);
    }

    //
    // Cleanup.
    //
    lws_context_destroy(context);

    return 0;
}



