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




#include <time.h>
#include <python3.5/Python.h>
#include <python3.5/frameobject.h>
#include <signal.h>
#include <time.h>




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
void MilliSleep(uint32_t milliseconds)
{
   struct timespec  tim;
   struct timespec  tim2;

   tim.tv_sec   = 0;
   tim.tv_nsec  = milliseconds*1000000;

   nanosleep(&tim , &tim2);    
}


//
//
//
int TraceFunc(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg)
{
    int   lineNumber    = PyFrame_GetLineNumber(frame);
    char* filename      = PyUnicode_AsUTF8(frame->f_code->co_filename);
    char* funcname      = PyUnicode_AsUTF8(frame->f_code->co_name);    

    printf("(TraceFunc %d @%s:%d %s)\n", what, filename,lineNumber,funcname);
    MilliSleep(100);

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
int main(int argc, char* argv[])
{
    //
    //
    //
    signal(SIGINT, intHandler);

    //
    // Setup the interpreter.
    //
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName(L"BlockWorks TestBench");
    Py_Initialize();


    //
    //
    //
    PyEval_SetTrace( &TraceFunc, NULL );

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



