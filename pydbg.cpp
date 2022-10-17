#include <cstddef>
#include <iostream>
#include <Python.h>
#include "get_func.h"
#include "set_string.h"
#include "division.h"
#include "test_myclass.h"

// https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/debug_in_ide.html




int main(int argc, char *argv[], char *envp[])
{
    // for (char **env = envp; *env != 0; env++)
    // {
    //     char *thisEnv = *env;
    //     printf("%s\n", thisEnv);    
    // }
    // return 0;
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    /* Add a built-in module, before Py_Initialize */
//    if (PyImport_AppendInittab("spam", PyInit_spam) == -1) {
//        fprintf(stderr, "Error: could not extend in-built modules table\n");
//        exit(1);
//    }

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required.
       If this step fails, it will be a fatal error. */
    Py_Initialize();

//    const char* script_path = R"(/home/stephan/Documents/PythonCppExtension/main.py)";
//    // https://docs.python.org/3/c-api/veryhigh.html#c.PyRun_SimpleFile
//    FILE* script = fopen(script_path, "r");
//    if(script){
//        PyRun_SimpleFile(script, "main.py");
//    }
//    fclose(script);

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    PyObject *pmodule = PyImport_ImportModule("mymath");
    if (!pmodule) {
        PyErr_Print();
        fprintf(stderr, "Error: could not import module 'mymath'\n");
        return -1;
    }

    test_division(get_func(pmodule, "division"));
    test_setString(get_func(pmodule, "setString"));
    test_myclass(get_func(pmodule, "init"));

    PyMem_RawFree(program);
    return 0;
}
