#include <Python.h>
#include <iostream>
#include <cstring>
#include <ostream>
#include <sstream>
#include "methodobject.h"
#include "modsupport.h"
#include "object.h"
#include "pyport.h"
#include "tupleobject.h"
#include "unicodeobject.h"
#include <structmember.h>

#include "my_class_py_type.h"

// Wichtig
// https://github.com/python/cpython/blob/main/Modules/_testmultiphase.c
// https://docs.python.org/3/extending/newtypes_tutorial.html
// https://github.com/python/cpython/blob/5a8c15819c27c516e5b75b7c9d89eacdb16b77c3/Modules/_testcapi/heaptype.c
//static MyClass* myclass;
// https://stackoverflow.com/questions/64922768/can-a-python-heap-time-created-with-pytype-fromspec-be-weak-reference-compat
// https://pythoncapi.readthedocs.io/type_object.html

// Memory Management
// https://docs.python.org/3/c-api/memory.html
// Debugging
// https://llllllllll.github.io/c-extension-tutorial/gdb.html





static PyObject* ExampleFunc(PyObject *self, PyObject *args) {
    std::stringstream ss;
    ss << "Current ref cnt: " << self->ob_refcnt << "\n";
    std::string str = ss.str();
    return PyUnicode_FromString(str.c_str());
}







// static PyObject *Init(PyObject *self, PyObject* args){
    
//     std::cout << "MyModule Init() called" << std::endl;
    
//     struct PyMemberDef MyClass_members[] = {
//     {"value", T_INT, offsetof(MyClassObject, value)},
//     //{"anotherObj", T_OBJECT, offsetof(MyClassObject, m_anotherObj)},
//     {NULL} /* Sentinel */
//     };

//     PyType_Slot MyClass_slots[] = {
//         // Possible slots
//         // https://docs.python.org/3/c-api/typeobj.html
//         // Slot examples
//         // https://docs.python.org/3/c-api/typeobj.html#typedef-examples
//         //{Py_tp_doc, (void*)PyDoc_STR("Custom object 123") },
//         //{Py_tp_dealloc, PyCursesPanel_Dealloc},
//         //{Py_tp_new, (void*)MyClass_new},
//         {Py_tp_setattro, (void*)PyObject_GenericSetAttr},
//         //{Py_tp_init, (void*)MyClass_init},
//         {Py_tp_dealloc, (void*)MyClass_Dealloc},
//         {Py_tp_members,  MyClass_members},
//         //{Py_tp_methods, MyClass_methods},
//         {0, 0},
//     };


//     PyType_Spec spec_myclass = {
//         "MyClass", // tp_name
//         // https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_basicsize
//         sizeof(MyClassObject), // tp_basicsize
//         0,
//         Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
//         MyClass_slots // slots
//     };

//     //int nerr = PyMember_SetOne()

//     // Only creates a type
//     PyObject *myclass = PyType_FromSpec(&spec_myclass);
//     if (myclass == NULL){
//         return NULL;
//     }
//     //Py_INCREF(myclass);
//     return myclass;

// }

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


// Exported methods are collected in a table
// https://docs.python.org/3/c-api/structures.html
PyMethodDef method_table[] = {
    {"division", (PyCFunction) division, METH_VARARGS, "Method docstring"},
    {"setString", (PyCFunction) setString, METH_VARARGS, "Write to passed string"},
    //{"init", (PyCFunction)},
    {NULL, NULL, 0, NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
PyModuleDef my_module = {
    PyModuleDef_HEAD_INIT,
    "MyModule", // Module name
    "This is MyModule's docstring",
    -1,   // Optional size of the module state memory
    //method_table,
    NULL, // Optional module methods
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};





PyMODINIT_FUNC 
PyInit_MyModule(void) {
    PyObject* module = PyModule_Create(&my_module);

    PyObject *myclass = PyType_FromSpec(&spec_myclass);
    if (myclass == NULL){
        return NULL;
    }
    Py_INCREF(myclass);

    if(PyModule_AddObject(module, "MyClass", myclass) < 0){
        Py_DECREF(myclass);
        Py_DECREF(module);
        return NULL;
    }
    return module;
}