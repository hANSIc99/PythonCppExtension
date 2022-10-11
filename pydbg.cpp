#include <cstddef>
#include <iostream>
#include <Python.h>

// https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/debug_in_ide.html

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

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    PyObject *pmodule = PyImport_ImportModule("mymath");
    if (!pmodule) {
        PyErr_Print();
        fprintf(stderr, "Error: could not import module 'mymath'\n");
    }

    //
    const char* sf_division = "division";
    PyObject* pFunc_division = PyObject_GetAttrString(pmodule, sf_division);
    if (!pFunc_division) {
        std::cerr << "Cannot find function " << sf_division << std::endl;
    }

    if (!PyCallable_Check(pFunc_division)) {
        std::cerr << "Function " << sf_division << " is not calable" << std::endl;
    }

//    PyObject *pResult = PyObject_CallNoArgs(pFunc_division);
//    if (!pResult) {
//        std::cerr << "Cannot call " << sf_division << " without arguments" << std::endl;
//    }

    PyObject *pResult = PyObject_CallObject(pFunc_division, Py_BuildValue("ll", 44, 2));
    if (!pResult) {
        std::cerr << "Cannot call " << sf_division << " with two integers" << std::endl;
    } else {
        long result = 0;
        // https://docs.python.org/3/c-api/long.html
        result = PyLong_AsLong(pResult);
        //PyArg_ParseTuple(pResult, "l", &result);
        std::cout << "The result is: " << result << std::endl;
    }

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
