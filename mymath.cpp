#include <Python.h>
#include <iostream>
#include <cstring>
#include "methodobject.h"
#include "myclass.h"
#include "object.h"
#include "pyport.h"
#include "tupleobject.h"
#include "unicodeobject.h"

// Wichtig
// https://github.com/python/cpython/blob/main/Modules/_testmultiphase.c
// https://docs.python.org/3/extending/newtypes_tutorial.html

static MyClass* myclass;
// https://stackoverflow.com/questions/64922768/can-a-python-heap-time-created-with-pytype-fromspec-be-weak-reference-compat
// https://pythoncapi.readthedocs.io/type_object.html


typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    // POINTER TO MYCLASS HERE ?
} MyClassObject;

static PyObject* ExampleFunc(PyObject *self, PyObject *args) {
    return PyUnicode_FromString("Example func called!");
}

static PyMethodDef Example_methods[] = {
    {"example", (PyCFunction)ExampleFunc,  METH_VARARGS | METH_CLASS, PyDoc_STR("example() -> None")},
    {NULL,              NULL}           /* sentinel */
};



static PyType_Slot PyMyClass_Type_slots[] = {
    // Possible slots
    // https://docs.python.org/3/c-api/typeobj.html
    // Slot examples
    // https://docs.python.org/3/c-api/typeobj.html#typedef-examples
    //{Py_tp_doc, (void*)PyDoc_STR("Custom object 123") },
    //{Py_tp_dealloc, PyCursesPanel_Dealloc},
    //{Py_tp_d}
    {Py_tp_methods, Example_methods},
    {0, 0},
};


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

    if(PyTuple_Check(args)){
        std::cout << "Object is of type Tuple" << std::endl;
    } else {
        std::cout << "Object is of NOT of type Tuple" << std::endl;
    }

    Py_ssize_t size = PyTuple_Size(args);
    std::cout << "size :" << size << std::endl;
    PyObject *argString = PyTuple_GetItem(args, 0);

    if(PyUnicode_Check(argString)){
        std::cout << "Object is of type string" << std::endl;
    } else {
        std::cout << "Object it NOT of type string" << std::endl;
    }
    Py_ssize_t length = PyUnicode_GET_LENGTH(argString);
    std::cout << "string length: " << length << std::endl;

    void *buf = PyUnicode_DATA(argString);
    const char* ssetString = "Verarscht";
    strcpy((char*)buf, ssetString);
    return PyUnicode_FromString("Hello World");
}

static PyObject* init(PyObject *self, PyObject *args){
    // https://docs.python.org/3/c-api/memory.html
    MyClass* myclass = (MyClass*)PyObject_Malloc(sizeof(MyClass));
    new (myclass) MyClass();

    PyType_Spec spec_myclass = {
        "mymath.DeviceManager", // tp_name
        // https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_basicsize
        sizeof(MyClassObject), // tp_basicsize
        0, // itemsize vor variable sized objects // TODO
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE | Py_TPFLAGS_BASETYPE,
        PyMyClass_Type_slots // slots
    };


    //PyObject *pypbjDevMan = PyType_F
    // later:
    //myclass->~MyClass();
    //PyObject_Free(myclass);
    // https://docs.python.org/3/c-api/typeobj.html#heap-types

    PyObject *temp = PyType_FromModuleAndSpec(self, &spec_myclass, NULL);
    if (temp == NULL) {
        return NULL;
    }
    if (PyModule_AddObject(self, "StateAccessType", temp) != 0) {
        Py_DECREF(temp);
        return NULL;
    }
    Py_INCREF(temp);
    return temp;
}

// Exported methods are collected in a table
// https://docs.python.org/3/c-api/structures.html
PyMethodDef method_table[] = {
    {"division", (PyCFunction) division, METH_VARARGS, "Method docstring"},
    {"init", (PyCFunction) init, METH_NOARGS, "Construct C++ Object"},
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
PyMODINIT_FUNC 
PyInit_mymath(void) {
    return PyModule_Create(&mymath_module);
}
