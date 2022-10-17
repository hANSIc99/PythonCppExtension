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
// Wichtig
// https://github.com/python/cpython/blob/main/Modules/_testmultiphase.c
// https://docs.python.org/3/extending/newtypes_tutorial.html
// https://github.com/python/cpython/blob/5a8c15819c27c516e5b75b7c9d89eacdb16b77c3/Modules/_testcapi/heaptype.c
//static MyClass* myclass;
// https://stackoverflow.com/questions/64922768/can-a-python-heap-time-created-with-pytype-fromspec-be-weak-reference-compat
// https://pythoncapi.readthedocs.io/type_object.html

// Debugging
// https://llllllllll.github.io/c-extension-tutorial/gdb.html

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

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    // POINTER TO MYCLASS HERE ?
    MyClass* m_myclass;
} MyClassObject;

static PyObject* ExampleFunc(PyObject *self, PyObject *args) {
    std::stringstream ss;
    ss << "Current ref cnt: " << self->ob_refcnt << "\n";
    std::string str = ss.str();
    return PyUnicode_FromString(str.c_str());
}

static PyMethodDef MyClass_methods[] = {
    {"example", (PyCFunction)ExampleFunc,  METH_VARARGS | METH_CLASS, PyDoc_STR("example() -> None")},
    {NULL,              NULL}           /* sentinel */
};

void MyClass_finalize(PyObject *self){
    std::cout << "Finalize called!!!" << std::endl;
    //MyClassObject* m = reinterpret_cast<MyClassObject*>(self);
    //m->m_myclass->~MyClass();
    //PyObject_Free(m->m_myclass);
};


static void MyClass_free(PyObject *self) {
    std::cout << "Free called!!!" << std::endl;
    //PyTypeObject *tp = Py_TYPE(self);
    // free references and buffers here
    //tp->tp_free(self);
    //Py_DECREF(tp);
};

int MyClass_init(PyObject *self, PyObject *args, PyObject *kwds){
    std::cout << "Init called!!!" << std::endl;
    return 0;
}

static void MyClass_Dealloc(MyClassObject *self){
    std::cout << "MyClass_dealloc() called!!!" << std::endl;
    PyTypeObject *tp = Py_TYPE(self);
    // free references and buffers here
    MyClassObject* m = reinterpret_cast<MyClassObject*>(self);
    m->m_myclass->~MyClass();
    PyObject_Free(m->m_myclass);

    tp->tp_free(self);
    Py_DECREF(tp);
};

static PyType_Slot PyMyClass_Type_slots[] = {
    // Possible slots
    // https://docs.python.org/3/c-api/typeobj.html
    // Slot examples
    // https://docs.python.org/3/c-api/typeobj.html#typedef-examples
    //{Py_tp_doc, (void*)PyDoc_STR("Custom object 123") },
    //{Py_tp_dealloc, PyCursesPanel_Dealloc},
    //{Py_tp_new, (void*)MyClass_new},
    {Py_tp_init, (void*)MyClass_init},
    {Py_tp_dealloc, (void*)MyClass_Dealloc},
    //{Py_tp_free, (void*)MyClass_free}, // wird von dealloc aufgerufen
    //{Py_tp_finalize, (void*)MyClass_finalize}, // wird nicht benoetigt
    {Py_tp_methods, MyClass_methods},
    {0, 0},
};

static PyType_Spec spec_myclass = {
    "MyModule.MyClass", // tp_name
    // https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_basicsize
    sizeof(MyClassObject), // tp_basicsize
    //0, // itemsize vor variable sized objects // TODO
    sizeof(MyClass),
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    PyMyClass_Type_slots // slots
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
    

    //MyClass* myclass = (MyClass*)PyObject_Malloc(sizeof(MyClass));
    //new (myclass) MyClass();




    //PyObject *pypbjDevMan = PyType_F
    // later:
    //myclass->~MyClass();
    //PyObject_Free(myclass);
    // https://docs.python.org/3/c-api/typeobj.html#heap-types

    //PyObject *temp = PyType_FromModuleAndSpec(self, &spec_myclass, NULL);
    // PyObject *temp = PyType_FromSpec(&spec_myclass);
    // if (temp == NULL) {
    //     std::cout << "MyClass heap creation failed" << std::endl;
    //     return NULL;
    // } else {
    //     std::cout << "MyClass creation successfull" << std::endl;
    //     MyClassObject* m = reinterpret_cast<MyClassObject*>(temp);
    //     m->m_myclass = (MyClass*)PyObject_Malloc(sizeof(MyClass));
    //     new (m->m_myclass) MyClass();
    //     //Py_INCREF(temp);
    //     //Py_DECREF(temp);
    //     //Py_DECREF(temp);
    //     //Py_DECREF(temp);
    // }
    // if (PyModule_AddObject(self, "StateAccessType", temp) != 0) {
    //     Py_DECREF(temp);
    //     return NULL;
    // }
    
    PyObject *HeapCType = PyType_FromSpec(&HeapCType_spec);
    // PyObject *subclass_bases = PyTuple_Pack(1, HeapCType);
    // if (subclass_bases == NULL) {
    //     return NULL;
    // }
    return HeapCType;
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

// The module init function
// Der interpreter sucht nach dieser funktion
PyMODINIT_FUNC 
PyInit_MyModule(void) {
    return PyModule_Create(&mymod_module);
}