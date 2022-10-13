#include <Python.h>
#include <iostream>
#include "myclass.h"

static MyClass* myclass;
// https://stackoverflow.com/questions/8066438/how-to-dynamically-create-a-derived-type-in-the-python-c-api TODO

// This is the definition of a method
static PyObject* division(PyObject *self, PyObject *args) {
    long dividend, divisor;
    if (!PyArg_ParseTuple(args, "ll", &dividend, &divisor)) {
        return NULL;
    }
    if (0 == divisor) {
        PyErr_Format(PyExc_ZeroDivisionError, "Dividing %d by zero!", dividend);
        return NULL;
    }
    return PyLong_FromLong(dividend / divisor);
}

static PyObject* setString(PyObject* self, PyObject *args){
    if(PyDict_Check(args)){
        std::cout << "Object is of type DICT" << std::endl;
    } else {
        std::cout << "Object is of NOT of type DICT" << std::endl;
    }

    return PyUnicode_FromString("Hello World");
}

static PyObject* init(PyObject *self, PyObject *args){
    myclass = new MyClass();
    return NULL; // Return new PyObject
}

// Exported methods are collected in a table
// https://docs.python.org/3/c-api/structures.html
PyMethodDef method_table[] = {
    {"division", (PyCFunction) division, METH_VARARGS, "Method docstring"},
    //{"init", (PyCFunction) init, METH_NOARGS, "Construct C++ Object"},
    {"setString", (PyCFunction) setString, METH_VARARGS, "Write to passed string"},
    {NULL, NULL, 0, NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
PyModuleDef mymath_module = {
    PyModuleDef_HEAD_INIT,
    "mymath", // Module name
    "This is the module docstring",
    -1,   // Optional size of the module state memory
    method_table,
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

// The module init function
PyMODINIT_FUNC PyInit_mymath(void) {
    return PyModule_Create(&mymath_module);
}
