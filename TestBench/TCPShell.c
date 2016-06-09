//
// Purpose of this shell is:
//  - Abstract the details of the debugger from the GUI (portable interface, not shell/stdio related).
//  - Provide isolation of the test script (multiple scripts shall not interfere with each other).
//  - Test reliability not linked to GUI.
//  - Provide watchdog type facilities for the test script (maybe this is better done outside?).
//  - Abstract the details of the interpreter (potentially use different Python API for old/new versions).
//  - Potentially run scripts simultaeously that need different interpreters/environments.
//  - Provide GUI reconnection facilities.
//  - Provide remote debug facility, potentially without a shell/stdio.
//




#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "Common.h"
#include "Shell.h"




//
//
//
void* ShellMain(void* args)
{
    int                 parentfd; /* parent socket */
    int                 childfd; /* child socket */
    int                 portno; /* port to listen on */
    int                 clientlen; /* byte size of client's address */
    struct sockaddr_in  serveraddr; /* server's addr */
    struct sockaddr_in  clientaddr; /* client addr */
    struct hostent*     hostp; /* client host info */
    char                buf[1024]; /* message buffer */
    char*               hostaddrp; /* dotted decimal host addr string */
    int                 optval; /* flag value for setsockopt */
    int                 n; /* message byte size */

    portno = 1234;

    /*
    * socket: create the parent socket
    */
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0)
    {
        error("ERROR opening socket");
    }

    /* setsockopt: Handy debugging trick that lets
    * us rerun the server immediately after we kill it;
    * otherwise we have to wait about 20 secs.
    * Eliminates "ERROR on binding: Address already in use" error.
    */
    optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    /*
    * build the server's Internet address
    */
    memset((char *) &serveraddr, 0, sizeof(serveraddr));

    /* this is an Internet address */
    serveraddr.sin_family = AF_INET;

    /* let the system figure out our IP address */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* this is the port we will listen on */
    serveraddr.sin_port = htons((unsigned short)portno);

    /*
    * bind: associate the parent socket with a port
    */
    if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        error("ERROR on binding");
    }

    /*
    * listen: make this socket ready to accept connection requests
    */
    if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */
    {
        error("ERROR on listen");
    }

    /*
    * main loop: wait for a connection request, echo input line,
    * then close connection.
    */
    clientlen = sizeof(clientaddr);
    while(true)
    {
        /*
        * accept: wait for a connection request
        */
        childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
        if (childfd < 0)
        {
          error("ERROR on accept");
        }

        /*
        * gethostbyaddr: determine who sent the message
        */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL)
        {
            error("ERROR on gethostbyaddr");
        }
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL)
        {
            error("ERROR on inet_ntoa\n");
        }
        printf("server established connection with %s (%s)\n",
        hostp->h_name, hostaddrp);

        do
        {
            /*
            * read: read input string from the client
            */
            memset(buf, 0, sizeof(buf) );
            n = read(childfd, buf, sizeof(buf) );
            if (n <= 0)
            {
                error("ERROR reading from socket");
            }
            printf("server received %d bytes: %s", n, buf);

            /*
            * write: echo the input string back to the client
            */
            n = write(childfd, buf, strlen(buf));
            if (n <= 0)
            {
                error("ERROR writing to socket");
            }

        } while(n > 0);

        //
        // UI Connection terminated.
        //
        printf("UI Socket closed.\n");

    }

    close(childfd);
}
