


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "libwebsockets.h"

#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>


int callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
    return 0;
}











int callback_dumb_increment(struct lws* wsi, enum lws_callback_reasons reason,
                            void* user, void* in, size_t len)
{
    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
        printf("connection established\n");
        break;

    case LWS_CALLBACK_RECEIVE:   // the funny part
        {
            printf("received request\n");

            char*   buf     = (char*)malloc(1024);

            strcpy(buf, "{ \"data\":[[11,22,33,44],[21,22,23,24],[31,32,33,34],[41,42,43,44]] }");

            // send response
            // just notice that we have to tell where exactly our response starts. That's
            // why there's `buf[LWS_SEND_BUFFER_PRE_PADDING]` and how long it is.
            // we know that our response has the same length as request because
            // it's the same message in reverse order.
            lws_write(wsi, buf, strlen(buf), LWS_WRITE_TEXT);
            // release memory back into the wild
            free(buf);
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
        callback_dumb_increment,   // callback
        0                          // we don't use any per session data

    },
    {
        NULL, NULL, 0   /* End of list */
    }
};






int main(int argc, char** argv)
{
    // server url will be http://localhost:9000
    int port = 9000;
    const char* interface = NULL;
    struct lws_context* context;
    // we're not using ssl
    const char* cert_path = NULL;
    const char* key_path = NULL;
    // no special options
    int opts = 0;
    struct lws_context_creation_info info;
    //
    //
    //
    memset( &info, 0, sizeof(info) );
    info.port           = port;
    info.iface          = interface;
    info.protocols      = protocols;
    // create libwebsocket context representing this server
    context = lws_create_context( &info );

    if (context == NULL)
    {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }

    printf("starting server...\n");

    // infinite loop, to end this server send SIGTERM. (CTRL+C)
    while (1)
    {
        lws_service(context, 50);
        // libwebsocket_service will process all waiting events with their
        // callback functions and then wait 50 ms.
        // (this is a single threaded webserver and this will keep our server
        // from generating load while there are not requests to process)
    }

    lws_context_destroy(context);
    return 0;
}



