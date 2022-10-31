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

// A struct contains the definition of a module
PyModuleDef my_module = {
    PyModuleDef_HEAD_INIT,
    "MyModule", // Module name
    "This is MyModule's docstring",
    -1,   // Optional size of the module state memory
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