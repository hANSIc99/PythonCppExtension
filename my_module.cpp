#include <Python.h>
#include <iostream>
#include <cstring>
#include <ostream>
#include <sstream>
#include "methodobject.h"
#include "myclass.h"
#include "object.h"
#include "pyport.h"
#include "tupleobject.h"
#include "unicodeobject.h"
#include <structmember.h>


typedef struct {
    PyObject_HEAD
    int value;
} HeapCTypeObject;

//static extern "C"
static int
heapctype_init(PyObject *self, PyObject *args, PyObject *kwargs)
{
    std::cout << "Init called" << std::endl;
    ((HeapCTypeObject *)self)->value = 10;
    return 0;
}

static struct PyMemberDef heapctype_members[] = {
    {"value", T_INT, offsetof(HeapCTypeObject, value)},
    {NULL} /* Sentinel */
};

static void
heapctype_dealloc(HeapCTypeObject *self)
{
    std::cout << "dealloc called" << std::endl;
    PyTypeObject *tp = Py_TYPE(self);
    PyObject_Free(self);
    Py_DECREF(tp);
}

PyDoc_STRVAR(heapctype__doc__,
"A heap type without GC, but with overridden dealloc.\n\n"
"The 'value' attribute is set to 10 in __init__.");

static PyType_Slot HeapCType_slots[] = {
    {Py_tp_init, (void*)heapctype_init},
    {Py_tp_members, heapctype_members},
    {Py_tp_dealloc, (void*)heapctype_dealloc},
    {Py_tp_doc, (char*)heapctype__doc__},
    {0, 0},
};

static PyType_Spec HeapCType_spec = {
    "MyModule.HeapCType",
    sizeof(HeapCTypeObject),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    HeapCType_slots
};

static PyObject* init(PyObject *self, PyObject *args){
    
    PyObject *HeapCType = PyType_FromSpec(&HeapCType_spec);

    return HeapCType;
}

PyMethodDef method_table[] = {

    {"init", (PyCFunction) init, METH_NOARGS, "Construct create new PyObject"},
    {NULL, NULL, 0, NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
PyModuleDef mymod_module = {
    PyModuleDef_HEAD_INIT,
    "MyModule", // Module name
    "This is MyModule's docstring",
    -1,   // Optional size of the module state memory
    method_table,
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

PyMODINIT_FUNC 
PyInit_MyModule(void) {
    return PyModule_Create(&mymod_module);
}