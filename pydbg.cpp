#include <cstddef>
#include <iostream>
#include <Python.h>
#include "get_func.h"
#include "object.h"
#include "set_string.h"
#include "division.h"
#include "test_myclass.h"

// https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/debug_in_ide.html




int main(int argc, char *argv[], char *envp[])
{
    Py_SetProgramName(L"DbgPythonCppExtension");
    Py_Initialize();

    PyObject *pmodule = PyImport_ImportModule("MyModule");
    if (!pmodule) {
        PyErr_Print();
        std::cerr << "Failed to import module MyModule" << std::endl;
        return -1;
    }

    PyObject *myClassType = PyObject_GetAttrString(pmodule, "MyClass");
    if (!myClassType) {
        std::cerr << "Unable to get type MyClass from MyModule" << std::endl;
        return -1;
    }

    PyObject *myClassInstance = PyObject_CallObject(myClassType, NULL);

    if (!myClassInstance) {
        std::cerr << "Instantioation of MyClass failed" << std::endl;
        return -1;
    }

    Py_DecRef(myClassInstance); // invoke deallocation
    return 0;
}
