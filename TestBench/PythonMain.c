



#include <time.h>
#include <python3.5/Python.h>
#include <python3.5/frameobject.h>
#include <gtk/gtk.h>



//
//
//
int gmain(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "TestBenchGUI.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "applicationwindow1"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}


//
// called when window is closed
//
void on_window_main_destroy()
{
    gtk_main_quit();
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
    sleep(1);

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
    gmain(argc, argv);

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



