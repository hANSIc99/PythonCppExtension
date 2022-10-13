#include <cstddef>
#include <iostream>
#include <Python.h>
#include "set_string.h"
#include "division.h"

// https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/debug_in_ide.html

PyObject* get_func(PyObject *pmodule, const char* fName){

    PyObject* pFunc = PyObject_GetAttrString(pmodule, fName);
    if (!pFunc) {
        std::cerr << "Cannot find function " << fName << std::endl;
        return NULL;
    }

    if (!PyCallable_Check(pFunc)) {
        std::cerr << "Function " << fName << " is not calable" << std::endl;
        return NULL;
    }
    return pFunc;
}


int main(int argc, char *argv[])
{
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
    }

    test_division(get_func(pmodule, "division"));
    test_setString(get_func(pmodule, "setString"));


    const char* sf_init = "init";
    PyObject* pFunc_init = PyObject_GetAttrString(pmodule, sf_init);
    if (!pFunc_init) {
        std::cerr << "Cannot find function " << sf_init << std::endl;
    }

    // https://docs.python.org/3.10/c-api/call.html#c.PyObject_CallObject
    PyObject *pResult_b = PyObject_CallObject(pFunc_init, NULL);
    if (!pResult_b) {
        std::cerr << "Cannot call " << sf_init << " with two integers" << std::endl;
    } else {
        std::cout << "Init called" << std::endl;
    }

    PyMem_RawFree(program);
    return 0;
}
