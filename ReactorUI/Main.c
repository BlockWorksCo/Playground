/*
 * libwebsockets-test-servet - libwebsockets test implementation
 *
 * Copyright (C) 2010-2016 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * The test apps are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

/*
 * libwebsockets-test-server - libwebsockets test implementation
 *
 * Copyright (C) 2010-2016 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * The test apps are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

#if defined(_WIN32) && defined(EXTERNAL_POLL)
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define poll(fdArray, fds, timeout)  WSAPoll((LPWSAPOLLFD)(fdArray), (ULONG)(fds), (INT)(timeout))
#endif

//#include "lws_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "libwebsockets.h"

#ifdef _WIN32
#include <io.h>
#include "gettimeofday.h"
#else
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#endif

extern int close_testing;
extern int max_poll_elements;

#ifdef EXTERNAL_POLL
extern struct lws_pollfd* pollfds;
extern int* fd_lookup;
extern int count_pollfds;
#endif
extern volatile int force_exit;
extern struct lws_context* context;
//extern char *resource_path;

extern void test_server_lock(int care);
extern void test_server_unlock(int care);

#ifndef __func__
#define __func__ __FUNCTION__
#endif

struct per_session_data__http
{
    lws_filefd_type fd;
#ifdef LWS_WITH_CGI
    struct lws_cgi_args args;
#endif
#if defined(LWS_WITH_CGI) || !defined(LWS_NO_CLIENT)
    int reason_bf;
#endif
    unsigned int client_finished: 1;
};

/*
 * one of these is auto-created for each connection and a pointer to the
 * appropriate instance is passed to the callback in the user parameter
 *
 * for this example protocol we use it to individualize the count for each
 * connection.
 */

struct per_session_data__dumb_increment
{
    int number;
};

struct per_session_data__lws_mirror
{
    struct lws* wsi;
    int ringbuffer_tail;
};

struct per_session_data__echogen
{
    size_t total;
    size_t total_rx;
    int fd;
    int fragsize;
    int wr;
};

struct per_session_data__lws_status
{
    struct per_session_data__lws_status* list;
    struct timeval tv_established;
    int last;
    char ip[270];
    char user_agent[512];
    const char* pos;
    int len;
};

extern int
callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user,
              void* in, size_t len);
extern int
callback_lws_mirror(struct lws* wsi, enum lws_callback_reasons reason,
                    void* user, void* in, size_t len);
extern int
callback_dumb_increment(struct lws* wsi, enum lws_callback_reasons reason,
                        void* user, void* in, size_t len);
extern int
callback_lws_echogen(struct lws* wsi, enum lws_callback_reasons reason,
                     void* user, void* in, size_t len);
extern int
callback_lws_status(struct lws* wsi, enum lws_callback_reasons reason,
                    void* user, void* in, size_t len);


extern void
dump_handshake_info(struct lws* wsi);















int close_testing;
int max_poll_elements;
int debug_level = 7;

#ifdef EXTERNAL_POLL
struct lws_pollfd* pollfds;
int* fd_lookup;
int count_pollfds;
#endif
volatile int force_exit = 0;
struct lws_context* context;
struct lws_plat_file_ops fops_plat;

/* http server gets files from this path */
//#define LOCAL_RESOURCE_PATH INSTALL_DATADIR"/libwebsockets-test-server"
//char *resource_path = LOCAL_RESOURCE_PATH;

/* singlethreaded version --> no locks */

void test_server_lock(int care)
{
}
void test_server_unlock(int care)
{
}

/*
 * This demo server shows how to use libwebsockets for one or more
 * websocket protocols in the same server
 *
 * It defines the following websocket protocols:
 *
 *  dumb-increment-protocol:  once the socket is opened, an incrementing
 *              ascii string is sent down it every 50ms.
 *              If you send "reset\n" on the websocket, then
 *              the incrementing number is reset to 0.
 *
 *  lws-mirror-protocol: copies any received packet to every connection also
 *              using this protocol, including the sender
 */

enum demo_protocols
{
    /* always first */
    PROTOCOL_HTTP = 0,

    PROTOCOL_DUMB_INCREMENT,
    PROTOCOL_LWS_MIRROR,
    PROTOCOL_LWS_ECHOGEN,
    PROTOCOL_LWS_STATUS,

    /* always last */
    DEMO_PROTOCOL_COUNT
};



const char* get_mimetype(const char* file)
{
    int n = strlen(file);

    if (n < 5)
    {
        return NULL;
    }

    if (!strcmp(&file[n - 4], ".ico"))
    {
        return "image/x-icon";
    }

    if (!strcmp(&file[n - 4], ".png"))
    {
        return "image/png";
    }

    if (!strcmp(&file[n - 5], ".html"))
    {
        return "text/html";
    }

    if (!strcmp(&file[n - 4], ".css"))
    {
        return "text/css";
    }

    return NULL;
}

/* this protocol server (always the first one) handles HTTP,
 *
 * Some misc callbacks that aren't associated with a protocol also turn up only
 * here on the first protocol server.
 */

int callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
    return 0;
}






int
callback_lws_status(struct lws* wsi, enum lws_callback_reasons reason,
                    void* user, void* in, size_t len)
{
#if 0
    struct per_session_data__lws_status* pss =
        (struct per_session_data__lws_status*)user,
        **pp;
    char name[128], rip[128];
    int m;

    switch (reason)
    {
    case LWS_CALLBACK_PROTOCOL_INIT:
        /*
         * Prepare the static server info
         */
        server_info_len = sprintf((char*)server_info,
                                  "\"version\":\"%s\","
                                  " \"hostname\":\"%s\"",
                                  lws_get_library_version(),
                                  lws_canonical_hostname(
                                      lws_get_context(wsi)));
        break;

    case LWS_CALLBACK_ESTABLISHED:
        /*
         * we keep a linked list of live pss, so we can walk it
         */
        pss->last = 0;
        pss->list = list;
        list = pss;
        live_wsi++;
        lws_get_peer_addresses(wsi, lws_get_socket_fd(wsi), name,
                               sizeof(name), rip, sizeof(rip));
        sprintf(pss->ip, "%s (%s)", name, rip);
        gettimeofday(&pss->tv_established, NULL);
        strcpy(pss->user_agent, "unknown");
        lws_hdr_copy(wsi, pss->user_agent, sizeof(pss->user_agent),
                     WSI_TOKEN_HTTP_USER_AGENT);
        update_status(wsi, pss);
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        m = lws_write(wsi, (unsigned char*)cache + LWS_PRE, cache_len,
                      LWS_WRITE_TEXT);

        if (m < server_info_len)
        {
            lwsl_err("ERROR %d writing to di socket\n", m);
            return -1;
        }

        break;

    case LWS_CALLBACK_CLOSED:
        /*
         * remove ourselves from live pss list
         */
        lwsl_err("CLOSING pss %p ********\n", pss);
        pp = &list;

        while (*pp)
        {
            if (*pp == pss)
            {
                *pp = pss->list;
                pss->list = NULL;
                live_wsi--;
                break;
            }

            pp = &((*pp)->list);
        }

        update_status(wsi, pss);
        break;

    default:
        break;
    }

#endif
    return 0;
}


int
callback_lws_echogen(struct lws* wsi, enum lws_callback_reasons reason,
                     void* user, void* in, size_t len)
{
#if 0
    unsigned char buf[LWS_PRE + 8192];
    struct per_session_data__echogen* pss =
        (struct per_session_data__echogen*)user;
    unsigned char* p = &buf[LWS_PRE];
    int n, m;

    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        pss->total = TOTAL;
        pss->fragsize = 2048;
        pss->total_rx = 0;
        sprintf((char*)buf, "%s/test.html", resource_path);
        pss->fd = open((char*)buf, LWS_O_RDONLY);

        if (pss->fd < 0)
        {
            lwsl_err("Failed to open %s\n", buf);
            return -1;
        }

        pss->wr = LWS_WRITE_TEXT | LWS_WRITE_NO_FIN;
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_CLOSED:
        if (pss->fd >= 0)
        {
            close(pss->fd);
        }

        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        //      pss->fragsize += 16;
        //      if (pss->fragsize >= 4096)
        //          pss->fragsize = 32;
        lwsl_err("%s: cb writeable, total left %ld\n", __func__, (long)pss->total);
        m = pss->fragsize;

        if ((size_t)m >=  pss->total)
        {
            m = (int)pss->total;
            pss->wr = LWS_WRITE_CONTINUATION; /* ie, FIN */
        }

        n = read(pss->fd, p, m);

        if (n < 0)
        {
            lwsl_err("failed read\n");
            return -1;
        }

        if (n < m)
        {
            lseek(pss->fd, 0, SEEK_SET);
            m = read(pss->fd, p + n, m - n);

            if (m < 0)
            {
                return -1;
            }
        }

        else
        {
            m = 0;
        }

        pss->total -= n + m;
        m = lws_write(wsi, p, n + m, pss->wr);

        if (m < n)
        {
            lwsl_err("ERROR %d writing to di socket\n", n);
            return -1;
        }

        if (!pss->total)
        {
            lwsl_err("Completed OK\n");
            break;
        }

        pss->wr = LWS_WRITE_CONTINUATION | LWS_WRITE_NO_FIN;
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_RECEIVE:
        pss->total_rx += len;
        lwsl_err("rx %ld\n", (long)pss->total_rx);

        if (pss->total_rx == TOTAL)
        {
            lws_close_reason(wsi, LWS_CLOSE_STATUS_NORMAL,
                             (unsigned char*)"done", 4);
            return -1;
        }

        break;

    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
        lwsl_notice("LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: len %d\n",
                    len);

        for (n = 0; n < (int)len; n++)
            lwsl_notice(" %d: 0x%02X\n", n,
                        ((unsigned char*)in)[n]);

        break;

    default:
        break;
    }

#endif
    return 0;
}



int
callback_lws_mirror(struct lws* wsi, enum lws_callback_reasons reason,
                    void* user, void* in, size_t len)
{
#if 0
    struct per_session_data__lws_mirror* pss =
        (struct per_session_data__lws_mirror*)user;
    int n, m;

    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        lwsl_info("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
        pss->ringbuffer_tail = ringbuffer_head;
        pss->wsi = wsi;
        break;

    case LWS_CALLBACK_PROTOCOL_DESTROY:
        lwsl_notice("%s: mirror protocol cleaning up\n", __func__);

        for (n = 0; n < sizeof ringbuffer / sizeof ringbuffer[0]; n++)
            if (ringbuffer[n].payload)
            {
                free(ringbuffer[n].payload);
            }

        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        if (close_testing)
        {
            break;
        }

        while (pss->ringbuffer_tail != ringbuffer_head)
        {
            m = ringbuffer[pss->ringbuffer_tail].len;
            n = lws_write(wsi, (unsigned char*)
                          ringbuffer[pss->ringbuffer_tail].payload +
                          LWS_PRE, m, LWS_WRITE_TEXT);

            if (n < 0)
            {
                lwsl_err("ERROR %d writing to mirror socket\n", n);
                return -1;
            }

            if (n < m)
            {
                lwsl_err("mirror partial write %d vs %d\n", n, m);
            }

            if (pss->ringbuffer_tail == (MAX_MESSAGE_QUEUE - 1))
            {
                pss->ringbuffer_tail = 0;
            }

            else
            {
                pss->ringbuffer_tail++;
            }

            if (((ringbuffer_head - pss->ringbuffer_tail) &
                    (MAX_MESSAGE_QUEUE - 1)) == (MAX_MESSAGE_QUEUE - 15))
                lws_rx_flow_allow_all_protocol(lws_get_context(wsi),
                                               lws_get_protocol(wsi));

            if (lws_send_pipe_choked(wsi))
            {
                lws_callback_on_writable(wsi);
                break;
            }
        }

        break;

    case LWS_CALLBACK_RECEIVE:
        if (((ringbuffer_head - pss->ringbuffer_tail) &
                (MAX_MESSAGE_QUEUE - 1)) == (MAX_MESSAGE_QUEUE - 1))
        {
            lwsl_err("dropping!\n");
            goto choke;
        }

        if (ringbuffer[ringbuffer_head].payload)
        {
            free(ringbuffer[ringbuffer_head].payload);
        }

        ringbuffer[ringbuffer_head].payload = malloc(LWS_PRE + len);
        ringbuffer[ringbuffer_head].len = len;
        memcpy((char*)ringbuffer[ringbuffer_head].payload +
               LWS_PRE, in, len);

        if (ringbuffer_head == (MAX_MESSAGE_QUEUE - 1))
        {
            ringbuffer_head = 0;
        }

        else
        {
            ringbuffer_head++;
        }

        if (((ringbuffer_head - pss->ringbuffer_tail) &
                (MAX_MESSAGE_QUEUE - 1)) != (MAX_MESSAGE_QUEUE - 2))
        {
            goto done;
        }

choke:
        lwsl_debug("LWS_CALLBACK_RECEIVE: throttling %p\n", wsi);
        lws_rx_flow_control(wsi, 0);
done:
        lws_callback_on_writable_all_protocol(lws_get_context(wsi),
                                              lws_get_protocol(wsi));
        break;

    /*
     * this just demonstrates how to use the protocol filter. If you won't
     * study and reject connections based on header content, you don't need
     * to handle this callback
     */

    case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
        dump_handshake_info(wsi);
        /* you could return non-zero here and kill the connection */
        break;

    default:
        break;
    }

#endif
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
            // create a buffer to hold our response
            // it has to have some pre and post padding. You don't need to care
            // what comes there, libwebsockets will do everything for you. For more info see
            // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597
            unsigned char* buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + len +
                                 LWS_SEND_BUFFER_POST_PADDING);
            int i;

            // pointer to `void *in` holds the incomming request
            // we're just going to put it in reverse order and put it in `buf` with
            // correct offset. `len` holds length of the request.
            for (i = 0; i < len; i++)
            {
                buf[LWS_SEND_BUFFER_PRE_PADDING + (len - 1) - i ] = ((char*) in)[i];
            }

            // log what we recieved and what we're going to send as a response.
            // that disco syntax `%.*s` is used to print just a part of our buffer
            // http://stackoverflow.com/questions/5189071/print-part-of-char-array
            printf("received data: %s, replying: %.*s\n", (char*) in, (int) len,
                   buf + LWS_SEND_BUFFER_PRE_PADDING);
            // send response
            // just notice that we have to tell where exactly our response starts. That's
            // why there's `buf[LWS_SEND_BUFFER_PRE_PADDING]` and how long it is.
            // we know that our response has the same length as request because
            // it's the same message in reverse order.
            lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_TEXT);
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


/* this shows how to override the lws file operations.  You don't need
 * to do any of this unless you have a reason (eg, want to serve
 * compressed files without decompressing the whole archive)
 */
static lws_filefd_type
test_server_fops_open(struct lws* wsi, const char* filename,
                      unsigned long* filelen, int flags)
{
    lws_filefd_type n;
    /* call through to original platform implementation */
    n = fops_plat.open(wsi, filename, filelen, flags);
    lwsl_notice("%s: opening %s, ret %ld, len %lu\n", __func__, filename,
                (long)n, *filelen);
    return n;
}

void sighandler(int sig)
{
    force_exit = 1;
    lws_cancel_service(context);
}

static const struct lws_extension exts[] =
{
    {
        "permessage-deflate",
        lws_extension_callback_pm_deflate,
        "permessage-deflate"
    },
    {
        "deflate-frame",
        lws_extension_callback_pm_deflate,
        "deflate_frame"
    },
    { NULL, NULL, NULL /* terminator */ }
};



static struct option options[] =
{
    { "help",   no_argument,        NULL, 'h' },
    { "debug",  required_argument,  NULL, 'd' },
    { "port",   required_argument,  NULL, 'p' },
    { "ssl",    no_argument,        NULL, 's' },
    { "allow-non-ssl",  no_argument,    NULL, 'a' },
    { "interface",  required_argument,  NULL, 'i' },
    { "closetest",  no_argument,        NULL, 'c' },
    { "ssl-cert",  required_argument,   NULL, 'C' },
    { "ssl-key",  required_argument,    NULL, 'K' },
    { "ssl-ca",  required_argument,     NULL, 'A' },
    { "libev",  no_argument,        NULL, 'e' },
#ifndef LWS_NO_DAEMONIZE
    { "daemonize",  no_argument,        NULL, 'D' },
#endif
    //{ "resource_path", required_argument,   NULL, 'r' },
    { NULL, 0, 0, 0 }
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



