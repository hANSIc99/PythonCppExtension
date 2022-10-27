#ifndef MY_CLASS_PY_TYPE_H
#define MY_CLASS_PY_TYPE_H

#include <Python.h>
#include <iostream>
#include <sstream>
#include <structmember.h>

#include "my_cpp_class.h"

typedef struct {
    PyObject_HEAD
    int         m_value;
    MyClass*    m_myclass;
} MyClassObject;

int MyClass_init(PyObject *self, PyObject *args, PyObject *kwds);
PyObject *MyClass_new(PyTypeObject *subtype, PyObject *args, PyObject *kwds);
void MyClass_Dealloc(MyClassObject *self);

static PyMethodDef MyClass_methods[] = {
    //{"example", (PyCFunction)ExampleFunc,  METH_VARARGS | METH_CLASS, PyDoc_STR("example() -> None")},
    {NULL,              NULL}           /* sentinel */
};




// Create normal object
static struct PyMemberDef MyClass_members[] = {
    {"value", T_INT, offsetof(MyClassObject, m_value)},
    //{"anotherObj", T_OBJECT, offsetof(MyClassObject, m_anotherObj)},
    {NULL} /* Sentinel */
};

static PyType_Slot MyClass_slots[] = {
    // Possible slots
    // https://docs.python.org/3/c-api/typeobj.html
    // Slot examples
    // https://docs.python.org/3/c-api/typeobj.html#typedef-examples
    //{Py_tp_doc, (void*)PyDoc_STR("Custom object 123") },
    //{Py_tp_dealloc, PyCursesPanel_Dealloc},
    {Py_tp_new, (void*)MyClass_new},
    //{Py_tp_setattro, (void*)PyObject_GenericSetAttr},
    {Py_tp_init, (void*)MyClass_init},
    {Py_tp_dealloc, (void*)MyClass_Dealloc},
    //{Py_tp_members,  MyClass_members},
    {Py_tp_methods, MyClass_methods},
    {0, 0},
};

static PyType_Spec spec_myclass = {
    "MyClass",                                  // name
    sizeof(MyClassObject),                      // basicsize
    0,                                          // itemsize
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   //flags
    MyClass_slots                               // slots
};







































// typedef struct {
//     PyObject_HEAD
//     int value;
// } AnotherObject;

// static PyObject* ExampleFuncB(PyObject *self, PyObject *args) {
//     std::stringstream ss;
//     ss << "Current ref cnt: " << self->ob_refcnt << "\n";
//     std::string str = ss.str();
//     return PyUnicode_FromString(str.c_str());
// }

// static struct PyMemberDef AnotherObject_members[] = {
//     {"value", T_INT, offsetof(AnotherObject, value)},
//     {NULL} /* Sentinel */
// };

// int AnotherObject_init(PyObject *self, PyObject *args, PyObject *kwds){
//     std::cout << "AnotherObject: Init() called!!!" << std::endl;
//     ((AnotherObject *)self)->value = 22;
//     return 0;
// }

// static PyMethodDef AnotherObject_methods[] = {
//     {"example", (PyCFunction)ExampleFuncB,  METH_VARARGS | METH_CLASS, PyDoc_STR("example() -> None")},
//     {NULL,              NULL}           /* sentinel */
// };

// static void AnotherObject_Dealloc(AnotherObject *self){
//     std::cout << "AnotherObject: Dealloc() called!!!" << std::endl;
//     PyTypeObject *tp = Py_TYPE(self);

//     tp->tp_free(self);
//     Py_DECREF(tp);
// };

// static PyType_Slot AnotherObject_slots[] = {
//     {Py_tp_init, (void*)AnotherObject_init},
//     {Py_tp_dealloc, (void*)AnotherObject_Dealloc},
//     {Py_tp_members,  AnotherObject_members},
//     {Py_tp_methods, AnotherObject_methods},
//     {0, 0},
// };

// PyType_Spec spec_another_object = {
//     "AnotherObject", // tp_name
//     // https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_basicsize
//     sizeof(AnotherObject), // tp_basicsize
//     0,
//     Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
//     AnotherObject_slots // slots
// };
#endif