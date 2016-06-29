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
#include "semaphore.h"
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Common.h"
#include "Shell.h"
#include <stdlib.h>


#define NUMBER_OF_ELEMENTS(a)   (sizeof(a)/sizeof(a[0]))



//
// Definition of a breakpoint.
//
typedef struct
{
    uint32_t    lineNumber;
    char        fileName[1024];

} Breakpoint;

//
// Array of breakpoints.
//
Breakpoint  breakpoints[128];
sem_t       breakpointSemaphore     = {0};

bool        breakOnNext             = false;
bool        stopped                 = false;

bool        running                 = false;

char        currentFilename[1024]   = {0};
uint32_t    currentLineNumber       = 0;

int         commandPipe[2]          = {0};

char        fileBreak[1024]         = {0};
bool        breakOnFileMatch        = false;

uint32_t    breakOnStackDepth       = (uint32_t)-1;
uint32_t    currentStackDepth       = 0;




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
uint32_t AddBreakpoint(char* fileName, uint32_t lineNumber)
{
    uint32_t    breakpointId    = (uint32_t)-1;

    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(breakpoints); i++)
    {
        if(breakpoints[i].lineNumber == (uint32_t)-1)
        {
            breakpoints[i].lineNumber   = lineNumber;
            strcpy(breakpoints[i].fileName, fileName);
            breakpointId    = i;

            break;
        }
    }

    return breakpointId;
}

//
//
//
void DeleteBreakpoint(uint32_t breakpointId)
{
    breakpoints[breakpointId].lineNumber   = (uint32_t)-1;
}



//
//
//
uint32_t LookupBreakpoint(char* fileName, uint32_t lineNumber)
{
    uint32_t    breakpointId    = (uint32_t)-1;

    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(breakpoints); i++)
    {
        if(breakpoints[i].lineNumber == lineNumber)
        {
#if 0
            char*   temp        = &fileName[0];
            char*   lastSlash   = strrchr(&fileName[0], '/');
            if(lastSlash != NULL)
            {
                temp    = lastSlash + 1;
            }

            //
            //
            //
            char*   temp2   = &breakpoints[i].fileName[0];
            lastSlash   = strrchr(temp2, '/');
            if(lastSlash != NULL)
            {
                temp2    = lastSlash + 1;
            }
            if(strcmp(temp, temp2) == 0)
            {
                breakpointId    = i;
                break;
            }
#else
            if(strcmp( &fileName[0], &breakpoints[i].fileName[0] ) == 0)
            {
                breakpointId    = i;
                break;
            }
#endif
        }
    }

    return breakpointId;
}


//
//
//
void DisplayLocals()
{
    //PyGILState_STATE gstate;
    //gstate = PyGILState_Ensure();

    //
    //
    //
    PyObject*   locals  = PyEval_GetLocals();
    if(locals != NULL)
    {
        if( PyDict_Check(locals) == true )
        {
            //printf("locals is a dict.\n");

            PyObject*   key     = 0;
            PyObject*   value   = 0;
            Py_ssize_t  pos     = 0;

            printf("{\n");

            while (PyDict_Next(locals, &pos, &key, &value))
            {
                PyObject*   keyStringRepresentation     = PyObject_Str(key);
                char*       keyText                     = PyUnicode_AsUTF8(keyStringRepresentation);
                if(keyText != NULL)
                {
                    PyObject*   valueStringRepresentation   = PyObject_Str(value);
                    if(valueStringRepresentation != NULL)
                    {
                        char*       valueText                   = PyUnicode_AsUTF8(valueStringRepresentation);
                        printf("\"%s\" : \"%s\",\n", keyText, valueText);
                        //printf("\t%s:%s\n", keyText, valueText);
                        Py_DECREF(valueStringRepresentation);
                    }
                    else
                    {
                        printf("\"%s\" : \"%s\",\n", keyText, "<None>");
                        //printf("\t%s:%s\n", keyText, "<None>");
                    }
                }

                Py_DECREF(keyStringRepresentation);
            }

            printf("}\n");
        }
        else
        {
            //printf("locals is NOT a dict.\n");
        }
    }
    else
    {
        //printf("Locals is NULL\n" );
    }

    /* Release the thread. No Python API allowed beyond this point. */
    //PyGILState_Release(gstate);
}


//
//
//
void ShowCallStack()
{
    //
    // Request a break and wait for it to happen.
    //
    /*
    breakOnNext     = true;
    while(stopped == false)
    {
        usleep(100000);
    }
    */

    PyThreadState*  tstate = PyThreadState_GET();
    if (NULL != tstate && NULL != tstate->frame)
    {
        PyFrameObject*  frame = tstate->frame;

        printf("[\n");
        while (NULL != frame)
        {
            int line = frame->f_lineno;
            const char *filename = PyUnicode_AsUTF8(frame->f_code->co_filename);
            //const char *funcname = PyUnicode_AsUTF8(frame->f_code->co_name);
            printf("  {\n");
            printf("    \"fileName\" : \"%s\",\n", filename);
            printf("    \"lineNumber\" : \"%d\"\n", line);
            printf("  },\n");
            frame = frame->f_back;
        }
        printf("]\n");
    }
}



//
// Walk the stack to work out its depth.... yes, we can do this more efficiently...
//
uint32_t CallStackDepth()
{
    PyThreadState*  tstate      = PyThreadState_GET();
    uint32_t        stackDepth  = 0;

    if (NULL != tstate && NULL != tstate->frame)
    {
        PyFrameObject*  frame = tstate->frame;

        while (NULL != frame)
        {
            frame = frame->f_back;
            stackDepth++;
        }
    }

    return stackDepth;
}



//
// Waits for a command via the UI.
//
void WaitForCommand()
{
    //printf("<break at %s:%d>\n", filename, lineNumber);

    //
    // Stop and wait for user input.
    //
    stopped     = true;
    sem_wait(&breakpointSemaphore);
    stopped     = false;

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
    strcpy( &currentFilename[0], filename );
    currentLineNumber   = lineNumber;

    //
    //
    //
    switch(what)
    {
        case PyTrace_CALL:
            //printf("(PyTrace_CALL %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_EXCEPTION:
            //printf("(PyTrace_EXCEPTION %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_LINE:
        {
            //printf("(PyTrace_LINE %d @%s:%d %s)\n", what, filename,lineNumber,funcname);

            printf("{\n\"type\":\"PositionReport\",\n\"fileName\":\"%s\",\n\"lineNumber\":\"%d\"\n}\n", filename, lineNumber);

            //
            // Calculate the current stack depth.
            //
            currentStackDepth   = CallStackDepth();

            //
            // See if we need to break or not.
            //
            if( LookupBreakpoint(filename, lineNumber) != (uint32_t)-1 )
            {
                WaitForCommand();
            }

            if(breakOnNext == true)
            {
                WaitForCommand();
            }

            if((breakOnFileMatch == true) && (strcmp(filename, fileBreak) == 0))
            {
                WaitForCommand();
            }

            if( breakOnStackDepth == CallStackDepth() )
            {
                WaitForCommand();
            }

            //
            // Reset the temporary break conditions.
            //
            breakOnFileMatch    = false;
            break;
        }

        case PyTrace_RETURN:
            //printf("(PyTrace_RETURN %d @%s:%d %s)\n", what, filename,lineNumber,funcname);

            //
            // See if we need to break or not.
            //
            if( breakOnStackDepth == CallStackDepth() )
            {
                //printf("<break at %s:%d>\n", filename, lineNumber);

                //
                // Stop and wait for user input.
                //
                stopped     = true;
                sem_wait(&breakpointSemaphore);
                stopped     = false;

                //
                // Reset the temporary break conditions.
                //
                currentStackDepth   = (uint32_t)-1;
            }

            break;

        case PyTrace_C_CALL:
            //printf("(PyTrace_C_CALL %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_C_EXCEPTION:
            //printf("(PyTrace_C_EXCEPTION %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        case PyTrace_C_RETURN:
            //printf("(PyTrace_C_RETURN %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
            break;

        default:
            break;
    }

    //printf("(TraceFunc %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
    //usleep(10000);

    return 0;
}



//
//
//
void ProcessRequest(char* request)
{
    char    response[1024];

    if(strcmp(request, "quit") == 0)
    {
        //
        // Quit the shell/app/debugger.
        //
        exit(-1);
    }
    else if(strcmp(request, "curpos") == 0)
    {
        //
        // Report the current position.
        //
        sprintf(response, "%s:%d", currentFilename, currentLineNumber);
        //ProcessResponse(response);
    }
    else if(strcmp(request, "go") == 0)
    {
        //
        // Start the script.
        //
        breakOnNext     = false;
        uint32_t    command     = 1;
        write( commandPipe[1], &command, sizeof(command) );
    }
    else if(strncmp(request, "b ", 2) == 0)
    {
        //
        // Add a breakpoint to the list.
        //
        char        fileName[1024]  = {0};
        uint32_t    lineNumber      = (uint32_t)-1;
        sscanf(request, "b %s %d", &fileName[0], &lineNumber);
        char*       colon           = strchr( &fileName[0], ':' );
        if(colon != NULL)
        {
            *colon  = 0;
        }
        uint32_t    breakpointId    = AddBreakpoint( &fileName[0], lineNumber );
        //printf("BreakpointID (%s:%d) = %d.", fileName, lineNumber, breakpointId);

        //ProcessResponse("Breakpoint added.");
    }
    else if(strncmp(request, "del ", 4) == 0)
    {
        //
        // Remove a breakpoint from the list.
        //
        char        fileName[1024]  = {0};
        uint32_t    lineNumber      = (uint32_t)-1;
        sscanf(request, "del %s %d", &fileName[0], &lineNumber);
        char*       colon           = strchr( &fileName[0], ':' );
        if(colon != NULL)
        {
            *colon  = 0;
        }
        uint32_t    breakpointId    = LookupBreakpoint( &fileName[0], lineNumber );
        if(breakpointId != (uint32_t)-1)
        {
            DeleteBreakpoint(breakpointId);
            //printf("BreakpointID (%s:%d) = %d deleted.", fileName, lineNumber, breakpointId);

            //ProcessResponse("Breakpoint deleted.");
        }
        else
        {
            //ProcessResponse("Breakpoint deletion failed.");
        }
    }
    else if(strcmp(request, "cont") == 0)
    {
        //
        // Give the breakpointSemaphore.
        //
        breakOnNext         = false;
        breakOnFileMatch    = false;
        sem_post(&breakpointSemaphore);
    }
    else if(strcmp(request, "locals") == 0)
    {
        //
        // Respond with the locals.
        //
        DisplayLocals();
        uint32_t    command     = 1;
        write( commandPipe[1], &command, sizeof(command) );
        //ProcessResponse("Locals:");
    }
    else if(strcmp(request, "break") == 0)
    {
        //
        // Set the breakOnNext flag;
        //
        breakOnNext     = true;
    }
    else if(strcmp(request, "step") == 0)
    {
        //
        // Set the breakOnNext flag and let the script run;
        //
        breakOnNext     = true;
        sem_post(&breakpointSemaphore);

        //
        // Start the script.
        // In this case, we need to set the breakOnFileMatch to stop at the first
        // line in the file.
        //
        if(running == false)
        {
            breakOnNext         = false;
            breakOnFileMatch    = true;
            uint32_t    command     = 1;
            write( commandPipe[1], &command, sizeof(command) );
        }
    }
    else if(strcmp(request, "next") == 0)
    {
        //
        // Set the breakOnNext flag and let the script run;
        //
        breakOnStackDepth   = currentStackDepth;
        sem_post(&breakpointSemaphore);

        //
        // Start the script.
        // In this case, we need to set the breakOnFileMatch to stop at the first
        // line in the file.
        //
        if(running == false)
        {
            breakOnNext         = false;
            breakOnFileMatch    = true;
            uint32_t    command     = 1;
            write( commandPipe[1], &command, sizeof(command) );
        }
    }
    else if(strcmp(request, "stack") == 0)
    {
        //
        // Show the call stack.
        //
        stopped     = false;
        ShowCallStack();
    }
    else
    {
        //
        // Unknown request.
        //
        //ProcessResponse(request);
    }
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
    char    fullPath[PATH_MAX]  = {0};

    //
    // Save the specified script as the file break (for an immediate step break).
    //
    realpath( argv[1], &fullPath[0] );
    strcpy( &fileBreak[0], &fullPath[0] );

    //
    // Make sure we behave with CTRL-C.
    //
    signal(SIGINT, intHandler);

    //
    // Create the command pipe before the Shell thread.
    //
    setbuf(stdout, NULL);
    if (pipe(commandPipe) == -1)
    {
        exit(-1);
    }

    //
    // Create the UIServer thread.
    //
    memset(&breakpoints[0], 0xff, sizeof(breakpoints));
    sem_init(&breakpointSemaphore, 0, 0);
    pthread_create(&serverThreadID, NULL, &ShellMain, NULL);

    //
    // Setup the interpreter.
    //
    setenv("PYTHONPATH",".",1);
    //PySys_SetArgv();
    //Py_SetPythonHome();
    //Py_GetVersion();
    //Py_GetPlatform();
    //Py_GetCopyright();
    //Py_GetCompiler();
    //Py_GetBuildInfo();
    //Py_GetProgramFullPath();
    Py_SetProgramName(L"BlockWorks TestBench");
    Py_Initialize();
    PyEval_InitThreads();

    //
    //
    //
    //PyObject* myModuleString    = PyUnicode_FromString("SequenceOne");
    //PyObject* myModule          = PyImport_Import(myModuleString);

    //
    // Hook into the interpreter.
    //
    PyEval_SetTrace( &TraceFunc, NULL );


    if(argc == 2)
    {
        printf("{\n\"type\":\"Announce\",\n\"Message\":\"TestBench starting\"\n}\n");
    }
    else
    {
        printf("{\n\"type\":\"Announce\",\n\"Message\":\"TestBench failed to start\"\n}\n");
        exit(-1);
    }

    //
    //
    //
    while(true)
    {
        uint32_t    command     = 0;
        while (read( commandPipe    [0], &command, sizeof(command) ) > 0)
        {
            //printf("<%d>\n", command);

            switch(command)
            {
                case 1:
                    printf("{\n\"type\":\"StateChange\",\n\"newState\":\"Executing\"\n}\n");

                    //
                    //
                    //
                    if(running == false)
                    {
                        //PyObject* myFunction        = PyObject_GetAttrString(myModule,"Blaa");
                        //PyObject* args              = PyTuple_Pack(1, PyLong_FromLong(123) );
                        //PyObject* myResult          = PyObject_CallObject(myFunction, args);

                        FILE*   scriptFile  = fopen( &fullPath[0], "r" );
                        if(scriptFile != NULL)
                        {
                            running     = true;
                            PyRun_SimpleFile( scriptFile, &fullPath[0] );
                            running     = false;
                            printf("{\n\"type\":\"StateChange\",\n\"newState\":\"ExecutionComplete\"\n}\n");
                        }
                        else
                        {
                            running     = false;
                            printf("{\n\"type\":\"StateChange\",\n\"newState\":\"LoadScriptFailure\"\n}\n");
                        }
                    }

                    break;

                default:
                    printf("<Unknown command>\n");
            }
        }

        usleep(100000);
    }


    //
    // Clean up.
    //
    Py_Finalize();

    return 0;
}
