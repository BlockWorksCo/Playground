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
#include "Common.h"
#include "Shell.h"


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

    //printf("\n\n%p %p %p\n", frame->f_localsplus[1], frame->f_globals, frame->f_builtins);
#if 0
    //
    //
    //
    if(frame->f_localsplus[1] != NULL)
    {
        if( PyDict_Check(frame->f_localsplus[1]) == true )
        {
            printf("locals is a dict.\n");

            PyObject*   key     = 0;
            PyObject*   value   = 0;
            Py_ssize_t  pos     = 0;

            while (PyDict_Next(frame->f_localsplus[1], &pos, &key, &value))
            {
                char*   keyText     = PyUnicode_AsUTF8(key);
                char*   valueText   = PyUnicode_AsUTF8(value);
                //char*   posText     = PyUnicode_AsUTF8(pos);
                //if(keyText!=NULL && valueText!=NULL)
                {
                    printf("%s:%s\n", keyText, valueText);
                }
            }
        }
        else
        {
            printf("locals is NOT a dict.\n");
        }
    }
#endif
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
    usleep(10000);

    return 0;
}



//
//
//
void ProcessRequest(char* request)
{
    ProcessResponse(request);
}


//
//
//
void ProcessResponse(char* response)
{
    printf("[%s]\n",response);
}







//
//
//
void error(char *msg)
{
  perror(msg);
}



pthread_t   serverThreadID  = {0};



//
//
//
int main(int argc, char* argv[])
{
    //
    // Make sure we behave with CTRL-C.
    //
    signal(SIGINT, intHandler);

    //
    // Create the UIServer thread.
    //
    pthread_create(&serverThreadID, NULL, &ShellMain, NULL);

    //
    // Setup the interpreter.
    //
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName(L"BlockWorks TestBench");
    Py_Initialize();


    //
    // Hook into the interpreter.
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
    //
    //
    printf("<Execution finished>\n");
    while(true)
    {
        usleep(100000);
    }


    //
    // Clean up.
    //
    Py_Finalize();

    return 0;
}
