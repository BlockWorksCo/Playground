




#include <python3.5/Python.h>




//
//
//
int main(int argc, char* argv[])
{
    //
    // Setup the interpreter.
    //
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName(L"BlockWorks TestBench");
    Py_Initialize();

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

    //
    // Clean up.
    //
    Py_Finalize();

    return 0;
}



