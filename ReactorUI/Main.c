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
extern struct lws_pollfd *pollfds;
extern int *fd_lookup;
extern int count_pollfds;
#endif
extern volatile int force_exit;
extern struct lws_context *context;
//extern char *resource_path;

extern void test_server_lock(int care);
extern void test_server_unlock(int care);

#ifndef __func__
#define __func__ __FUNCTION__
#endif

struct per_session_data__http {
    lws_filefd_type fd;
#ifdef LWS_WITH_CGI
    struct lws_cgi_args args;
#endif
#if defined(LWS_WITH_CGI) || !defined(LWS_NO_CLIENT)
    int reason_bf;
#endif
    unsigned int client_finished:1;
};

/*
 * one of these is auto-created for each connection and a pointer to the
 * appropriate instance is passed to the callback in the user parameter
 *
 * for this example protocol we use it to individualize the count for each
 * connection.
 */

struct per_session_data__dumb_increment {
    int number;
};

struct per_session_data__lws_mirror {
    struct lws *wsi;
    int ringbuffer_tail;
};

struct per_session_data__echogen {
    size_t total;
    size_t total_rx;
    int fd;
    int fragsize;
    int wr;
};

struct per_session_data__lws_status {
    struct per_session_data__lws_status *list;
    struct timeval tv_established;
    int last;
    char ip[270];
    char user_agent[512];
    const char *pos;
    int len;
};

extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user,
          void *in, size_t len);
extern int
callback_lws_mirror(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len);
extern int
callback_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len);
extern int
callback_lws_echogen(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len);
extern int
callback_lws_status(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len);


extern void
dump_handshake_info(struct lws *wsi);















int close_testing;
int max_poll_elements;
int debug_level = 7;

#ifdef EXTERNAL_POLL
struct lws_pollfd *pollfds;
int *fd_lookup;
int count_pollfds;
#endif
volatile int force_exit = 0;
struct lws_context *context;
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

enum demo_protocols {
    /* always first */
    PROTOCOL_HTTP = 0,

    PROTOCOL_DUMB_INCREMENT,
    PROTOCOL_LWS_MIRROR,
    PROTOCOL_LWS_ECHOGEN,
    PROTOCOL_LWS_STATUS,

    /* always last */
    DEMO_PROTOCOL_COUNT
};




/* this protocol server (always the first one) handles HTTP,
 *
 * Some misc callbacks that aren't associated with a protocol also turn up only
 * here on the first protocol server.
 */

int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user,
          void *in, size_t len)
{
#if 0    
    struct per_session_data__http *pss =
            (struct per_session_data__http *)user;
    unsigned char buffer[4096 + LWS_PRE];
    unsigned long amount, file_len, sent;
    char leaf_path[1024];
    const char *mimetype;
    char *other_headers;
    unsigned char *end;
    struct timeval tv;
    unsigned char *p;
#ifndef LWS_NO_CLIENT
    struct per_session_data__http *pss1;
    struct lws *wsi1;
#endif
    char buf[256];
    char b64[64];
    int n, m;
#ifdef EXTERNAL_POLL
    struct lws_pollargs *pa = (struct lws_pollargs *)in;
#endif

    switch (reason) {
    case LWS_CALLBACK_HTTP:

        if (debug_level & LLL_INFO) {
            dump_handshake_info(wsi);

            /* dump the individual URI Arg parameters */
            n = 0;
            while (lws_hdr_copy_fragment(wsi, buf, sizeof(buf),
                             WSI_TOKEN_HTTP_URI_ARGS, n) > 0) {
                lwsl_notice("URI Arg %d: %s\n", ++n, buf);
            }
        }

        {
            char name[100], rip[50];
            lws_get_peer_addresses(wsi, lws_get_socket_fd(wsi), name,
                           sizeof(name), rip, sizeof(rip));
            sprintf(buf, "%s (%s)", name, rip);
            lwsl_notice("HTTP connect from %s\n", buf);
        }

        if (len < 1) {
            lws_return_http_status(wsi,
                        HTTP_STATUS_BAD_REQUEST, NULL);
            goto try_to_reuse;
        }

#ifndef LWS_NO_CLIENT
        if (!strncmp(in, "/proxytest", 10)) {
            struct lws_client_connect_info i;
            char *rootpath = "/";
            const char *p = (const char *)in;

            if (lws_get_child(wsi))
                break;

            pss->client_finished = 0;
            memset(&i,0, sizeof(i));
            i.context = lws_get_context(wsi);
            i.address = "git.libwebsockets.org";
            i.port = 80;
            i.ssl_connection = 0;
            if (p[10])
                i.path = (char *)in + 10;
            else
                i.path = rootpath;
            i.host = "git.libwebsockets.org";
            i.origin = NULL;
            i.method = "GET";
            i.parent_wsi = wsi;
            i.uri_replace_from = "git.libwebsockets.org/";
            i.uri_replace_to = "/proxytest/";
            if (!lws_client_connect_via_info(&i)) {
                lwsl_err("proxy connect fail\n");
                break;
            }



            break;
        }
#endif

#if 1
        /* this example server has no concept of directories */
        if (strchr((const char *)in + 1, '/')) {
            lws_return_http_status(wsi, HTTP_STATUS_FORBIDDEN, NULL);
            goto try_to_reuse;
        }
#endif

#ifdef LWS_WITH_CGI
        if (!strncmp(in, "/cgitest", 8)) {
            static char *cmd[] = {
                "/bin/sh",
                "-c",
                INSTALL_DATADIR"/libwebsockets-test-server/lws-cgi-test.sh",
//              "/var/www/cgi-bin/cgit",
                NULL
            };

            lwsl_notice("%s: cgitest\n", __func__);
            n = lws_cgi(wsi, cmd, 8, 5);
            if (n) {
                lwsl_err("%s: cgi failed\n");
                return -1;
            }
            p = buffer + LWS_PRE;
            end = p + sizeof(buffer) - LWS_PRE;

            if (lws_add_http_header_status(wsi, 200, &p, end))
                return 1;
            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_CONNECTION,
                    (unsigned char *)"close", 5, &p, end))
                return 1;
            n = lws_write(wsi, buffer + LWS_PRE,
                      p - (buffer + LWS_PRE),
                      LWS_WRITE_HTTP_HEADERS);

            /* the cgi starts by outputting headers, we can't
             *  finalize the headers until we see the end of that
             */

            break;
        }
#endif

        /* if a legal POST URL, let it continue and accept data */
        if (lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI))
            return 0;

        /* check for the "send a big file by hand" example case */

        if (!strcmp((const char *)in, "/leaf.jpg")) {
            if (strlen(resource_path) > sizeof(leaf_path) - 10)
                return -1;
            sprintf(leaf_path, "%s/leaf.jpg", resource_path);

            /* well, let's demonstrate how to send the hard way */

            p = buffer + LWS_PRE;
            end = p + sizeof(buffer) - LWS_PRE;

            pss->fd = lws_plat_file_open(wsi, leaf_path, &file_len,
                             LWS_O_RDONLY);

            if (pss->fd == LWS_INVALID_FILE) {
                lwsl_err("faild to open file %s\n", leaf_path);
                return -1;
            }

            /*
             * we will send a big jpeg file, but it could be
             * anything.  Set the Content-Type: appropriately
             * so the browser knows what to do with it.
             *
             * Notice we use the APIs to build the header, which
             * will do the right thing for HTTP 1/1.1 and HTTP2
             * depending on what connection it happens to be working
             * on
             */
            if (lws_add_http_header_status(wsi, 200, &p, end))
                return 1;
            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_SERVER,
                        (unsigned char *)"libwebsockets",
                    13, &p, end))
                return 1;
            if (lws_add_http_header_by_token(wsi,
                    WSI_TOKEN_HTTP_CONTENT_TYPE,
                        (unsigned char *)"image/jpeg",
                    10, &p, end))
                return 1;
            if (lws_add_http_header_content_length(wsi,
                                   file_len, &p,
                                   end))
                return 1;
            if (lws_finalize_http_header(wsi, &p, end))
                return 1;

            /*
             * send the http headers...
             * this won't block since it's the first payload sent
             * on the connection since it was established
             * (too small for partial)
             *
             * Notice they are sent using LWS_WRITE_HTTP_HEADERS
             * which also means you can't send body too in one step,
             * this is mandated by changes in HTTP2
             */

            *p = '\0';
            lwsl_info("%s\n", buffer + LWS_PRE);

            n = lws_write(wsi, buffer + LWS_PRE,
                      p - (buffer + LWS_PRE),
                      LWS_WRITE_HTTP_HEADERS);
            if (n < 0) {
                lws_plat_file_close(wsi, pss->fd);
                return -1;
            }
            /*
             * book us a LWS_CALLBACK_HTTP_WRITEABLE callback
             */
            lws_callback_on_writable(wsi);
            break;
        }

        /* if not, send a file the easy way */
        if (!strncmp(in, "/cgit-data/", 11)) {
            in = (char *)in + 11;
            strcpy(buf, "/usr/share/cgit");
        } else
            strcpy(buf, resource_path);

        if (strcmp(in, "/")) {
            if (*((const char *)in) != '/')
                strcat(buf, "/");
            strncat(buf, in, sizeof(buf) - strlen(buf) - 1);
        } else /* default file to serve */
            strcat(buf, "/test.html");
        buf[sizeof(buf) - 1] = '\0';

        /* refuse to serve files we don't understand */
        mimetype = get_mimetype(buf);
        if (!mimetype) {
            lwsl_err("Unknown mimetype for %s\n", buf);
            lws_return_http_status(wsi,
                      HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, NULL);
            return -1;
        }

        /* demonstrates how to set a cookie on / */

        other_headers = leaf_path;
        p = (unsigned char *)leaf_path;
        if (!strcmp((const char *)in, "/") &&
               !lws_hdr_total_length(wsi, WSI_TOKEN_HTTP_COOKIE)) {
            /* this isn't very unguessable but it'll do for us */
            gettimeofday(&tv, NULL);
            n = sprintf(b64, "test=LWS_%u_%u_COOKIE;Max-Age=360000",
                (unsigned int)tv.tv_sec,
                (unsigned int)tv.tv_usec);

            if (lws_add_http_header_by_name(wsi,
                (unsigned char *)"set-cookie:",
                (unsigned char *)b64, n, &p,
                (unsigned char *)leaf_path + sizeof(leaf_path)))
                return 1;
        }
        if (lws_is_ssl(wsi) && lws_add_http_header_by_name(wsi,
                        (unsigned char *)
                        "Strict-Transport-Security:",
                        (unsigned char *)
                        "max-age=15768000 ; "
                        "includeSubDomains", 36, &p,
                        (unsigned char *)leaf_path +
                            sizeof(leaf_path)))
            return 1;
        n = (char *)p - leaf_path;

        n = lws_serve_http_file(wsi, buf, mimetype, other_headers, n);
        if (n < 0 || ((n > 0) && lws_http_transaction_completed(wsi)))
            return -1; /* error or can't reuse connection: close the socket */

        /*
         * notice that the sending of the file completes asynchronously,
         * we'll get a LWS_CALLBACK_HTTP_FILE_COMPLETION callback when
         * it's done
         */
        break;

    case LWS_CALLBACK_HTTP_BODY:
        strncpy(buf, in, 20);
        buf[20] = '\0';
        if (len < 20)
            buf[len] = '\0';

        lwsl_notice("LWS_CALLBACK_HTTP_BODY: %s... len %d\n",
                (const char *)buf, (int)len);

        break;

    case LWS_CALLBACK_HTTP_BODY_COMPLETION:
        lwsl_notice("LWS_CALLBACK_HTTP_BODY_COMPLETION\n");
        /* the whole of the sent body arrived, close or reuse the connection */
        lws_return_http_status(wsi, HTTP_STATUS_OK, NULL);
        goto try_to_reuse;

    case LWS_CALLBACK_HTTP_FILE_COMPLETION:
        goto try_to_reuse;

    case LWS_CALLBACK_HTTP_WRITEABLE:
        lwsl_info("LWS_CALLBACK_HTTP_WRITEABLE\n");

        if (pss->client_finished)
            return -1;

        if (pss->fd == LWS_INVALID_FILE)
            goto try_to_reuse;
#ifdef LWS_WITH_CGI
        if (pss->reason_bf & 1) {
            if (lws_cgi_write_split_stdout_headers(wsi) < 0)
                goto bail;

            pss->reason_bf &= ~1;
            break;
        }
#endif
#ifndef LWS_NO_CLIENT
        if (pss->reason_bf & 2) {
            char *px = buf + LWS_PRE;
            int lenx = sizeof(buf) - LWS_PRE;
            /*
             * our sink is writeable and our source has something
             * to read.  So read a lump of source material of
             * suitable size to send or what's available, whichever
             * is the smaller.
             */
            pss->reason_bf &= ~2;
            wsi1 = lws_get_child(wsi);
            if (!wsi1)
                break;
            if (lws_http_client_read(wsi1, &px, &lenx) < 0)
                goto bail;

            if (pss->client_finished)
                return -1;
            break;
        }
#endif
        /*
         * we can send more of whatever it is we were sending
         */
        sent = 0;
        do {
            /* we'd like the send this much */
            n = sizeof(buffer) - LWS_PRE;

            /* but if the peer told us he wants less, we can adapt */
            m = lws_get_peer_write_allowance(wsi);

            /* -1 means not using a protocol that has this info */
            if (m == 0)
                /* right now, peer can't handle anything */
                goto later;

            if (m != -1 && m < n)
                /* he couldn't handle that much */
                n = m;

            n = lws_plat_file_read(wsi, pss->fd,
                           &amount, buffer + LWS_PRE, n);
            /* problem reading, close conn */
            if (n < 0) {
                lwsl_err("problem reading file\n");
                goto bail;
            }
            n = (int)amount;
            /* sent it all, close conn */
            if (n == 0)
                goto penultimate;
            /*
             * To support HTTP2, must take care about preamble space
             *
             * identification of when we send the last payload frame
             * is handled by the library itself if you sent a
             * content-length header
             */
            m = lws_write(wsi, buffer + LWS_PRE, n, LWS_WRITE_HTTP);
            if (m < 0) {
                lwsl_err("write failed\n");
                /* write failed, close conn */
                goto bail;
            }
            if (m) /* while still active, extend timeout */
                lws_set_timeout(wsi, PENDING_TIMEOUT_HTTP_CONTENT, 5);
            sent += m;

        } while (!lws_send_pipe_choked(wsi) && (sent < 1024 * 1024));
later:
        lws_callback_on_writable(wsi);
        break;
penultimate:
        lws_plat_file_close(wsi, pss->fd);
        pss->fd = LWS_INVALID_FILE;
        goto try_to_reuse;

bail:
        lws_plat_file_close(wsi, pss->fd);

        return -1;

    /*
     * callback for confirming to continue with client IP appear in
     * protocol 0 callback since no websocket protocol has been agreed
     * yet.  You can just ignore this if you won't filter on client IP
     * since the default unhandled callback return is 0 meaning let the
     * connection continue.
     */
    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
        /* if we returned non-zero from here, we kill the connection */
        break;

#ifndef LWS_NO_CLIENT
    case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP: {
        char ctype[64], ctlen = 0;
        lwsl_err("LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP\n");
        p = buffer + LWS_PRE;
        end = p + sizeof(buffer) - LWS_PRE;
        if (lws_add_http_header_status(lws_get_parent(wsi), 200, &p, end))
            return 1;
        if (lws_add_http_header_by_token(lws_get_parent(wsi),
                WSI_TOKEN_HTTP_SERVER,
                    (unsigned char *)"libwebsockets",
                13, &p, end))
            return 1;

        ctlen = lws_hdr_copy(wsi, ctype, sizeof(ctype), WSI_TOKEN_HTTP_CONTENT_TYPE);
        if (ctlen > 0) {
            if (lws_add_http_header_by_token(lws_get_parent(wsi),
                WSI_TOKEN_HTTP_CONTENT_TYPE,
                (unsigned char *)ctype, ctlen, &p, end))
                return 1;
        }
#if 0
        if (lws_add_http_header_content_length(lws_get_parent(wsi),
                               file_len, &p, end))
            return 1;
#endif
        if (lws_finalize_http_header(lws_get_parent(wsi), &p, end))
            return 1;

        *p = '\0';
        lwsl_info("%s\n", buffer + LWS_PRE);

        n = lws_write(lws_get_parent(wsi), buffer + LWS_PRE,
                  p - (buffer + LWS_PRE),
                  LWS_WRITE_HTTP_HEADERS);
        if (n < 0)
            return -1;

        break; }
    case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
        //lwsl_err("LWS_CALLBACK_CLOSED_CLIENT_HTTP\n");
        return -1;
        break;
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
        //lwsl_err("LWS_CALLBACK_RECEIVE_CLIENT_HTTP: wsi %p\n", wsi);
        assert(lws_get_parent(wsi));
        if (!lws_get_parent(wsi))
            break;
        // lwsl_err("LWS_CALLBACK_RECEIVE_CLIENT_HTTP: wsi %p: sock: %d, parent_wsi: %p, parent_sock:%d,  len %d\n",
        //      wsi, lws_get_socket_fd(wsi),
        //      lws_get_parent(wsi),
        //      lws_get_socket_fd(lws_get_parent(wsi)), len);
        pss1 = lws_wsi_user(lws_get_parent(wsi));
        pss1->reason_bf |= 2;
        lws_callback_on_writable(lws_get_parent(wsi));
        break;
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
        //lwsl_err("LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ len %d\n", len);
        assert(lws_get_parent(wsi));
        m = lws_write(lws_get_parent(wsi), (unsigned char *)in,
                len, LWS_WRITE_HTTP);
        if (m < 0)
            return -1;
        break;
    case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
        //lwsl_err("LWS_CALLBACK_COMPLETED_CLIENT_HTTP\n");
        assert(lws_get_parent(wsi));
        if (!lws_get_parent(wsi))
            break;
        pss1 = lws_wsi_user(lws_get_parent(wsi));
        pss1->client_finished = 1;
        break;
#endif

#ifdef LWS_WITH_CGI
    /* CGI IO events (POLLIN/OUT) appear here our demo user code policy is
     *
     *  - POST data goes on subprocess stdin
     *  - subprocess stdout goes on http via writeable callback
     *  - subprocess stderr goes to the logs
     */
    case LWS_CALLBACK_CGI:
        pss->args = *((struct lws_cgi_args *)in);
        //lwsl_notice("LWS_CALLBACK_CGI: ch %d\n", pss->args.ch);
        switch (pss->args.ch) { /* which of stdin/out/err ? */
        case LWS_STDIN:
            /* TBD stdin rx flow control */
            break;
        case LWS_STDOUT:
            pss->reason_bf |= 1;
            /* when writing to MASTER would not block */
            lws_callback_on_writable(wsi);
            break;
        case LWS_STDERR:
            n = read(lws_get_socket_fd(pss->args.stdwsi[LWS_STDERR]),
                    buf, 127);
            //lwsl_notice("stderr reads %d\n", n);
            if (n > 0) {
                if (buf[n - 1] != '\n')
                    buf[n++] = '\n';
                buf[n] = '\0';
                lwsl_notice("CGI-stderr: %s\n", buf);
            }
            break;
        }
        break;

    case LWS_CALLBACK_CGI_TERMINATED:
        //lwsl_notice("LWS_CALLBACK_CGI_TERMINATED\n");
        /* because we sent on openended http, close the connection */
        return -1;

    case LWS_CALLBACK_CGI_STDIN_DATA:  /* POST body for stdin */
        //lwsl_notice("LWS_CALLBACK_CGI_STDIN_DATA\n");
        pss->args = *((struct lws_cgi_args *)in);
        n = write(lws_get_socket_fd(pss->args.stdwsi[LWS_STDIN]),
              pss->args.data, pss->args.len);
        //lwsl_notice("LWS_CALLBACK_CGI_STDIN_DATA: write says %d", n);
        if (n < pss->args.len)
            lwsl_notice("LWS_CALLBACK_CGI_STDIN_DATA: sent %d only %d went",
                    n, pss->args.len);
        return n;
#endif

    /*
     * callbacks for managing the external poll() array appear in
     * protocol 0 callback
     */

    case LWS_CALLBACK_LOCK_POLL:
        /*
         * lock mutex to protect pollfd state
         * called before any other POLL related callback
         * if protecting wsi lifecycle change, len == 1
         */
        test_server_lock(len);
        break;

    case LWS_CALLBACK_UNLOCK_POLL:
        /*
         * unlock mutex to protect pollfd state when
         * called after any other POLL related callback
         * if protecting wsi lifecycle change, len == 1
         */
        test_server_unlock(len);
        break;

#ifdef EXTERNAL_POLL
    case LWS_CALLBACK_ADD_POLL_FD:

        if (count_pollfds >= max_poll_elements) {
            lwsl_err("LWS_CALLBACK_ADD_POLL_FD: too many sockets to track\n");
            return 1;
        }

        fd_lookup[pa->fd] = count_pollfds;
        pollfds[count_pollfds].fd = pa->fd;
        pollfds[count_pollfds].events = pa->events;
        pollfds[count_pollfds++].revents = 0;
        break;

    case LWS_CALLBACK_DEL_POLL_FD:
        if (!--count_pollfds)
            break;
        m = fd_lookup[pa->fd];
        /* have the last guy take up the vacant slot */
        pollfds[m] = pollfds[count_pollfds];
        fd_lookup[pollfds[count_pollfds].fd] = m;
        break;

    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
            pollfds[fd_lookup[pa->fd]].events = pa->events;
        break;
#endif

    case LWS_CALLBACK_GET_THREAD_ID:
        /*
         * if you will call "lws_callback_on_writable"
         * from a different thread, return the caller thread ID
         * here so lws can use this information to work out if it
         * should signal the poll() loop to exit and restart early
         */

        /* return pthread_getthreadid_np(); */

        break;

    default:
        break;
    }

    return 0;

    /* if we're on HTTP1.1 or 2.0, will keep the idle connection alive */
try_to_reuse:
    if (lws_http_transaction_completed(wsi))
        return -1;
#endif
    return 0;
}






int
callback_lws_status(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len)
{
#if 0    
    struct per_session_data__lws_status *pss =
            (struct per_session_data__lws_status *)user,
            **pp;
    char name[128], rip[128];
    int m;

    switch (reason) {

    case LWS_CALLBACK_PROTOCOL_INIT:
        /*
         * Prepare the static server info
         */
        server_info_len = sprintf((char *)server_info,
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
        m = lws_write(wsi, (unsigned char *)cache + LWS_PRE, cache_len,
                  LWS_WRITE_TEXT);
        if (m < server_info_len) {
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
        while (*pp) {
            if (*pp == pss) {
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
callback_lws_echogen(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len)
{
#if 0
    unsigned char buf[LWS_PRE + 8192];
    struct per_session_data__echogen *pss =
            (struct per_session_data__echogen *)user;
    unsigned char *p = &buf[LWS_PRE];
    int n, m;

    switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
        pss->total = TOTAL;
        pss->fragsize = 2048;
        pss->total_rx = 0;
        sprintf((char *)buf, "%s/test.html", resource_path);
        pss->fd = open((char *)buf, LWS_O_RDONLY);
        if (pss->fd < 0) {
            lwsl_err("Failed to open %s\n", buf);
            return -1;
        }
        pss->wr = LWS_WRITE_TEXT | LWS_WRITE_NO_FIN;
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_CLOSED:
        if (pss->fd >= 0)
            close(pss->fd);
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:

//      pss->fragsize += 16;
//      if (pss->fragsize >= 4096)
//          pss->fragsize = 32;

        lwsl_err("%s: cb writeable, total left %ld\n", __func__, (long)pss->total);
        m = pss->fragsize;
        if ((size_t)m >=  pss->total) {
            m = (int)pss->total;
            pss->wr = LWS_WRITE_CONTINUATION; /* ie, FIN */
        }
        n = read(pss->fd, p, m);
        if (n < 0) {
            lwsl_err("failed read\n");
            return -1;
        }
        if (n < m) {
            lseek(pss->fd, 0, SEEK_SET);
            m = read(pss->fd, p + n, m - n);
            if (m < 0)
                return -1;
        } else
            m = 0;
        pss->total -= n + m;
        m = lws_write(wsi, p, n + m, pss->wr);
        if (m < n) {
            lwsl_err("ERROR %d writing to di socket\n", n);
            return -1;
        }
        if (!pss->total) {
            lwsl_err("Completed OK\n");
            break;
        }
        pss->wr = LWS_WRITE_CONTINUATION | LWS_WRITE_NO_FIN;
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_RECEIVE:
        pss->total_rx += len;
        lwsl_err("rx %ld\n", (long)pss->total_rx);
        if (pss->total_rx == TOTAL) {
            lws_close_reason(wsi, LWS_CLOSE_STATUS_NORMAL,
                     (unsigned char *)"done", 4);
            return -1;
        }
        break;

    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
        lwsl_notice("LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: len %d\n",
                len);
        for (n = 0; n < (int)len; n++)
            lwsl_notice(" %d: 0x%02X\n", n,
                    ((unsigned char *)in)[n]);
        break;

    default:
        break;
    }
#endif
    return 0;
}



int
callback_lws_mirror(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len)
{
#if 0    
    struct per_session_data__lws_mirror *pss =
            (struct per_session_data__lws_mirror *)user;
    int n, m;

    switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
        lwsl_info("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
        pss->ringbuffer_tail = ringbuffer_head;
        pss->wsi = wsi;
        break;

    case LWS_CALLBACK_PROTOCOL_DESTROY:
        lwsl_notice("%s: mirror protocol cleaning up\n", __func__);
        for (n = 0; n < sizeof ringbuffer / sizeof ringbuffer[0]; n++)
            if (ringbuffer[n].payload)
                free(ringbuffer[n].payload);
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        if (close_testing)
            break;
        while (pss->ringbuffer_tail != ringbuffer_head) {
            m = ringbuffer[pss->ringbuffer_tail].len;
            n = lws_write(wsi, (unsigned char *)
                   ringbuffer[pss->ringbuffer_tail].payload +
                   LWS_PRE, m, LWS_WRITE_TEXT);
            if (n < 0) {
                lwsl_err("ERROR %d writing to mirror socket\n", n);
                return -1;
            }
            if (n < m)
                lwsl_err("mirror partial write %d vs %d\n", n, m);

            if (pss->ringbuffer_tail == (MAX_MESSAGE_QUEUE - 1))
                pss->ringbuffer_tail = 0;
            else
                pss->ringbuffer_tail++;

            if (((ringbuffer_head - pss->ringbuffer_tail) &
                (MAX_MESSAGE_QUEUE - 1)) == (MAX_MESSAGE_QUEUE - 15))
                lws_rx_flow_allow_all_protocol(lws_get_context(wsi),
                           lws_get_protocol(wsi));

            if (lws_send_pipe_choked(wsi)) {
                lws_callback_on_writable(wsi);
                break;
            }
        }
        break;

    case LWS_CALLBACK_RECEIVE:
        if (((ringbuffer_head - pss->ringbuffer_tail) &
            (MAX_MESSAGE_QUEUE - 1)) == (MAX_MESSAGE_QUEUE - 1)) {
            lwsl_err("dropping!\n");
            goto choke;
        }

        if (ringbuffer[ringbuffer_head].payload)
            free(ringbuffer[ringbuffer_head].payload);

        ringbuffer[ringbuffer_head].payload = malloc(LWS_PRE + len);
        ringbuffer[ringbuffer_head].len = len;
        memcpy((char *)ringbuffer[ringbuffer_head].payload +
               LWS_PRE, in, len);
        if (ringbuffer_head == (MAX_MESSAGE_QUEUE - 1))
            ringbuffer_head = 0;
        else
            ringbuffer_head++;

        if (((ringbuffer_head - pss->ringbuffer_tail) &
            (MAX_MESSAGE_QUEUE - 1)) != (MAX_MESSAGE_QUEUE - 2))
            goto done;

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








int
callback_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len)
{
#if 0    
    unsigned char buf[LWS_PRE + 512];
    struct per_session_data__dumb_increment *pss =
            (struct per_session_data__dumb_increment *)user;
    unsigned char *p = &buf[LWS_PRE];
    int n, m;

    switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
        pss->number = 0;
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        n = sprintf((char *)p, "%d", pss->number++);
        m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
        if (m < n) {
            lwsl_err("ERROR %d writing to di socket\n", n);
            return -1;
        }
        if (close_testing && pss->number == 50) {
            lwsl_info("close tesing limit, closing\n");
            return -1;
        }
        break;

    case LWS_CALLBACK_RECEIVE:
        if (len < 6)
            break;
        if (strcmp((const char *)in, "reset\n") == 0)
            pss->number = 0;
        if (strcmp((const char *)in, "closeme\n") == 0) {
            lwsl_notice("dumb_inc: closing as requested\n");
            lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
                     (unsigned char *)"seeya", 5);
            return -1;
        }
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

    /*
     * this just demonstrates how to handle
     * LWS_CALLBACK_WS_PEER_INITIATED_CLOSE and extract the peer's close
     * code and auxiliary data.  You can just not handle it if you don't
     * have a use for this.
     */
    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
        lwsl_notice("LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: len %d\n",
                len);
        for (n = 0; n < (int)len; n++)
            lwsl_notice(" %d: 0x%02X\n", n,
                    ((unsigned char *)in)[n]);
        break;

    default:
        break;
    }
#endif
    return 0;
}






/* list of supported protocols and callbacks */

static struct lws_protocols protocols[] = {
    /* first protocol must always be HTTP handler */

    {
        "http-only",        /* name */
        callback_http,      /* callback */
        sizeof (struct per_session_data__http), /* per_session_data_size */
        0,          /* max frame size / rx buffer */
    },
    {
        "dumb-increment-protocol",
        callback_dumb_increment,
        sizeof(struct per_session_data__dumb_increment),
        10,
    },
    {
        "lws-mirror-protocol",
        callback_lws_mirror,
        sizeof(struct per_session_data__lws_mirror),
        128,
    },
    {
        "lws-echogen",
        callback_lws_echogen,
        sizeof(struct per_session_data__echogen),
        128,
    },
    {
        "lws-status",
        callback_lws_status,
        sizeof(struct per_session_data__lws_status),
        128,
    },
    { NULL, NULL, 0, 0 } /* terminator */
};


/* this shows how to override the lws file operations.  You don't need
 * to do any of this unless you have a reason (eg, want to serve
 * compressed files without decompressing the whole archive)
 */
static lws_filefd_type
test_server_fops_open(struct lws *wsi, const char *filename,
              unsigned long *filelen, int flags)
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

static const struct lws_extension exts[] = {
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



static struct option options[] = {
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





int main(int argc, char **argv)
{
    struct lws_context_creation_info info;
    char interface_name[128] = "";
    unsigned int ms, oldms = 0;
    const char *iface = NULL;
    char cert_path[1024] = "";
    char key_path[1024] = "";
    char ca_path[1024] = "";
    int uid = -1, gid = -1;
    int use_ssl = 0;
    int opts = 0;
    int n = 0;
#ifndef _WIN32
    int syslog_options = LOG_PID | LOG_PERROR;
#endif
#ifndef LWS_NO_DAEMONIZE
    int daemonize = 0;
#endif

    /*
     * take care to zero down the info struct, he contains random garbaage
     * from the stack otherwise
     */
    memset(&info, 0, sizeof info);
    info.port = 7681;

    while (n >= 0) {
        n = getopt_long(argc, argv, "eci:hsap:d:Dr:C:K:A:u:g:", options, NULL);
        if (n < 0)
            continue;
        switch (n) {
        case 'e':
            opts |= LWS_SERVER_OPTION_LIBEV;
            break;
#ifndef LWS_NO_DAEMONIZE
        case 'D':
            daemonize = 1;
            #ifndef _WIN32
            syslog_options &= ~LOG_PERROR;
            #endif
            break;
#endif
        case 'u':
            uid = atoi(optarg);
            break;
        case 'g':
            gid = atoi(optarg);
            break;
        case 'd':
            debug_level = atoi(optarg);
            break;
        case 's':
            use_ssl = 1;
            break;
        case 'a':
            opts |= LWS_SERVER_OPTION_ALLOW_NON_SSL_ON_SSL_PORT;
            break;
        case 'p':
            info.port = atoi(optarg);
            break;
        case 'i':
            strncpy(interface_name, optarg, sizeof interface_name);
            interface_name[(sizeof interface_name) - 1] = '\0';
            iface = interface_name;
            break;
        case 'c':
            close_testing = 1;
            fprintf(stderr, " Close testing mode -- closes on "
                       "client after 50 dumb increments"
                       "and suppresses lws_mirror spam\n");
            break;
        case 'r':
            //resource_path = optarg;
            //printf("Setting resource path to \"%s\"\n", resource_path);
            break;
        case 'C':
            strncpy(cert_path, optarg, sizeof cert_path);
            break;
        case 'K':
            strncpy(key_path, optarg, sizeof key_path);
            break;
        case 'A':
            strncpy(ca_path, optarg, sizeof ca_path);
            break;
        case 'h':
            fprintf(stderr, "Usage: test-server "
                    "[--port=<p>] [--ssl] "
                    "[-d <log bitfield>] "
                    "[--resource_path <path>]\n");
            exit(1);
        }
    }

#if !defined(LWS_NO_DAEMONIZE) && !defined(WIN32)
    /*
     * normally lock path would be /var/lock/lwsts or similar, to
     * simplify getting started without having to take care about
     * permissions or running as root, set to /tmp/.lwsts-lock
     */
    if (daemonize && lws_daemonize("/tmp/.lwsts-lock")) {
        fprintf(stderr, "Failed to daemonize\n");
        return 10;
    }
#endif

    signal(SIGINT, sighandler);

#ifndef _WIN32
    /* we will only try to log things according to our debug_level */
    setlogmask(LOG_UPTO (LOG_DEBUG));
    openlog("lwsts", syslog_options, LOG_DAEMON);
#endif

    /* tell the library what debug level to emit and to send it to syslog */
    lws_set_log_level(debug_level, lwsl_emit_syslog);

    lwsl_notice("libwebsockets test server - license LGPL2.1+SLE\n");
    lwsl_notice("(C) Copyright 2010-2016 Andy Green <andy@warmcat.com>\n");

    //printf("Using resource path \"%s\"\n", resource_path);
#ifdef EXTERNAL_POLL
    max_poll_elements = getdtablesize();
    pollfds = malloc(max_poll_elements * sizeof (struct lws_pollfd));
    fd_lookup = malloc(max_poll_elements * sizeof (int));
    if (pollfds == NULL || fd_lookup == NULL) {
        lwsl_err("Out of memory pollfds=%d\n", max_poll_elements);
        return -1;
    }
#endif

    info.iface = iface;
    info.protocols = protocols;
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    if (use_ssl) {
#if 0
        if (strlen(resource_path) > sizeof(cert_path) - 32) {
            lwsl_err("resource path too long\n");
            return -1;
        }
        if (!cert_path[0])
            sprintf(cert_path, "%s/libwebsockets-test-server.pem",
                                resource_path);
        if (strlen(resource_path) > sizeof(key_path) - 32) {
            lwsl_err("resource path too long\n");
            return -1;
        }
        if (!key_path[0])
            sprintf(key_path, "%s/libwebsockets-test-server.key.pem",
                                resource_path);

        info.ssl_cert_filepath = cert_path;
        info.ssl_private_key_filepath = key_path;
        if (ca_path[0])
            info.ssl_ca_filepath = ca_path;
#else
        exit(-1);
#endif
    }
    info.gid = gid;
    info.uid = uid;
    info.max_http_header_pool = 16;
    info.options = opts | LWS_SERVER_OPTION_VALIDATE_UTF8;
    info.extensions = exts;
    info.timeout_secs = 5;
    info.ssl_cipher_list = "ECDHE-ECDSA-AES256-GCM-SHA384:"
                   "ECDHE-RSA-AES256-GCM-SHA384:"
                   "DHE-RSA-AES256-GCM-SHA384:"
                   "ECDHE-RSA-AES256-SHA384:"
                   "HIGH:!aNULL:!eNULL:!EXPORT:"
                   "!DES:!MD5:!PSK:!RC4:!HMAC_SHA1:"
                   "!SHA1:!DHE-RSA-AES128-GCM-SHA256:"
                   "!DHE-RSA-AES128-SHA256:"
                   "!AES128-GCM-SHA256:"
                   "!AES128-SHA256:"
                   "!DHE-RSA-AES256-SHA256:"
                   "!AES256-GCM-SHA384:"
                   "!AES256-SHA256";

    if (use_ssl)
        /* redirect guys coming on http */
        info.options |= LWS_SERVER_OPTION_REDIRECT_HTTP_TO_HTTPS;

    context = lws_create_context(&info);
    if (context == NULL) {
        lwsl_err("libwebsocket init failed\n");
        return -1;
    }

    /* this shows how to override the lws file operations.  You don't need
     * to do any of this unless you have a reason (eg, want to serve
     * compressed files without decompressing the whole archive)
     */
    /* stash original platform fops */
    fops_plat = *(lws_get_fops(context));
    /* override the active fops */
    lws_get_fops(context)->open = test_server_fops_open;

    n = 0;
    while (n >= 0 && !force_exit) {
        struct timeval tv;

        gettimeofday(&tv, NULL);

        /*
         * This provokes the LWS_CALLBACK_SERVER_WRITEABLE for every
         * live websocket connection using the DUMB_INCREMENT protocol,
         * as soon as it can take more packets (usually immediately)
         */

        ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
        if ((ms - oldms) > 50) {
            lws_callback_on_writable_all_protocol(context,
                &protocols[PROTOCOL_DUMB_INCREMENT]);
            oldms = ms;
        }

#ifdef EXTERNAL_POLL
        /*
         * this represents an existing server's single poll action
         * which also includes libwebsocket sockets
         */

        n = poll(pollfds, count_pollfds, 50);
        if (n < 0)
            continue;

        if (n)
            for (n = 0; n < count_pollfds; n++)
                if (pollfds[n].revents)
                    /*
                    * returns immediately if the fd does not
                    * match anything under libwebsockets
                    * control
                    */
                    if (lws_service_fd(context,
                                  &pollfds[n]) < 0)
                        goto done;
#else
        /*
         * If libwebsockets sockets are all we care about,
         * you can use this api which takes care of the poll()
         * and looping through finding who needed service.
         *
         * If no socket needs service, it'll return anyway after
         * the number of ms in the second argument.
         */

        n = lws_service(context, 50);
#endif
    }

#ifdef EXTERNAL_POLL
done:
#endif

    lws_context_destroy(context);

    lwsl_notice("libwebsockets-test-server exited cleanly\n");

#ifndef _WIN32
    closelog();
#endif

    return 0;
}



