//
// Purpose of this shell is:
//  - Abstract the details of the debugger from the GUI (portable interface, not shell/stdio related).
//  - Provide isolation of the test script (multiple scripts shall not interfere with each other).
//  - Provide watchdog type facilities for the test script (maybe this is better done outside?).
//  - Abstract the details of the interpreter (potentially use different Python API for old/new versions).
//  - Potentially run scripts simultaeously that need different interpreters/environments.
//  - Provide GUI reconnection facilities.
//  - Provide remote debug facility, potentially without a shell/stdio.
//




#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <python3.5/Python.h>
#include <python3.5/frameobject.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define BUFSIZE 1024


static volatile int keepRunning = 1;


//
//
//
void intHandler(int dummy) 
{
    exit(-1);
}



//
//
//
int TraceFunc(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg)
{
    int   lineNumber    = PyFrame_GetLineNumber(frame);
    char* filename      = PyUnicode_AsUTF8(frame->f_code->co_filename);
    char* funcname      = PyUnicode_AsUTF8(frame->f_code->co_name);    


    //
    //
    //
    switch(what)
    {
        case PyTrace_CALL:
            printf("(PyTrace_CALL %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_EXCEPTION:
            printf("(PyTrace_EXCEPTION %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_LINE:
            printf("(PyTrace_LINE %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_RETURN:
            printf("(PyTrace_RETURN %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_C_CALL:
            printf("(PyTrace_C_CALL %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_C_EXCEPTION:
            printf("(PyTrace_C_EXCEPTION %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_C_RETURN:
            printf("(PyTrace_C_RETURN %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        default:
            break;
    }

    //printf("(TraceFunc %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
    usleep(100000);

    return 0;
}



void UseTBDB()
{


    //
    // Import PDB.
    //
    PyObject* dbModuleString   = PyUnicode_FromString("tbdb");
    PyObject* dbModule         = PyImport_Import(dbModuleString);
    PyObject* dbClass          = PyObject_GetAttrString(dbModule,"tbdb");

    PyObject* dbFunction       = PyObject_GetAttrString(dbClass,"runcall");
    //PyObject* dbFunctionString = PyUnicode_FromString("Test");

    //
    // Create a new instance of the debugger.
    //
    PyObject* dbInstance       = PyObject_CallObject(dbClass, NULL);

    //
    // Import the sequence.
    //
    PyObject* myModuleString    = PyUnicode_FromString("SequenceOne");
    PyObject* myModule          = PyImport_Import(myModuleString);

    PyObject* myFunction        = PyObject_GetAttrString(myModule,"Blaa");
    //PyObject* args              = PyTuple_Pack(1,PyFloat_FromDouble(2.0));

    //PyObject* myResult          = PyObject_CallObject(myFunction, args);
    //double result               = PyFloat_AsDouble(myResult);

    //
    //
    //
    //PyObject* args              = PyTuple_Pack(2, myFunction, PyLong_FromLong(123) );
    //PyObject* myResult          = PyObject_CallObject(dbFunction, args);
    //PyObject* myResult          = PyObject_CallMethodObjArgs(dbInstance, dbFunctionString,  PyLong_FromLong(123), NULL);
    if(PyCallable_Check(dbFunction) != 0)
    {
        printf("is callable.\n");
        PyObject* args              = PyTuple_Pack(3, dbInstance, myFunction, PyLong_FromLong(123) );
        PyObject* myResult          = PyObject_CallObject(dbFunction, args);
    }
    else
    {
        printf("Not callable.\n");
    }

    //
    // Show the result.
    //
    //printf("result is %f\n", result);

}




//
//
//
/*
 * error - wrapper for perror
 */
void error(char *msg) 
{
  perror(msg);
  exit(1);
}



pthread_t   serverThreadID  = {0};


//
//
//
void* doSomeThing(void *arg)
{
    int                 parentfd; /* parent socket */
    int                 childfd; /* child socket */
    int                 portno; /* port to listen on */
    int                 clientlen; /* byte size of client's address */
    struct sockaddr_in  serveraddr; /* server's addr */
    struct sockaddr_in  clientaddr; /* client addr */
    struct hostent*     hostp; /* client host info */
    char                buf[BUFSIZE]; /* message buffer */
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
    bzero((char *) &serveraddr, sizeof(serveraddr));

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

        /* 
        * read: read input string from the client
        */
        bzero(buf, BUFSIZE);
        n = read(childfd, buf, BUFSIZE);
        if (n < 0) 
        {
            error("ERROR reading from socket");
        }
        printf("server received %d bytes: %s", n, buf);

        /* 
        * write: echo the input string back to the client 
        */
        n = write(childfd, buf, strlen(buf));
        if (n < 0) 
        {
            error("ERROR writing to socket");
        }

        close(childfd);
    }





    while(true)
    {
        //MilliSleep(1000);

        usleep(500000);
        printf("<tick>\n");
    }
}





//
//
//
int main(int argc, char* argv[])
{
    //
    //
    //
    signal(SIGINT, intHandler);

    //
    //
    //
    pthread_create(&serverThreadID, NULL, &doSomeThing, NULL);

    //
    // Setup the interpreter.
    //
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName(L"BlockWorks TestBench");
    Py_Initialize();


    //
    //
    //
    //PyEval_SetTrace( &TraceFunc, NULL );
    PyEval_SetProfile( &TraceFunc, NULL );

    //
    //
    //
    PyObject* myModuleString    = PyUnicode_FromString("SequenceOne");
    PyObject* myModule          = PyImport_Import(myModuleString);
    PyObject* myFunction        = PyObject_GetAttrString(myModule,"Blaa");
    PyObject* args              = PyTuple_Pack(1, PyLong_FromLong(123) );
    PyObject* myResult          = PyObject_CallObject(myFunction, args);


    //
    // Clean up.
    //
    Py_Finalize();

    return 0;
}



