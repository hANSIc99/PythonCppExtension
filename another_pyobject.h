#ifndef ANOTHER_PYOBJECT_H
#define ANOTHER_PYOBJECT_H

#include <Python.h>
#include <iostream>
#include <sstream>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    int value;
} AnotherObject;

static PyObject* ExampleFuncB(PyObject *self, PyObject *args) {
    std::stringstream ss;
    ss << "Current ref cnt: " << self->ob_refcnt << "\n";
    std::string str = ss.str();
    return PyUnicode_FromString(str.c_str());
}

static struct PyMemberDef AnotherObject_members[] = {
    {"value", T_INT, offsetof(AnotherObject, value)},
    {NULL} /* Sentinel */
};

int AnotherObject_init(PyObject *self, PyObject *args, PyObject *kwds){
    std::cout << "AnotherObject: Init() called!!!" << std::endl;
    ((AnotherObject *)self)->value = 10;
    return 0;
}

static PyMethodDef AnotherObject_methods[] = {
    {"example", (PyCFunction)ExampleFuncB,  METH_VARARGS | METH_CLASS, PyDoc_STR("example() -> None")},
    {NULL,              NULL}           /* sentinel */
};

static void AnotherObject_Dealloc(AnotherObject *self){
    std::cout << "AnotherObject: Dealloc() called!!!" << std::endl;
    PyTypeObject *tp = Py_TYPE(self);

    tp->tp_free(self);
    Py_DECREF(tp);
};

static PyType_Slot AnotherObject_slots[] = {
    {Py_tp_init, (void*)AnotherObject_init},
    {Py_tp_dealloc, (void*)AnotherObject_Dealloc},
    {Py_tp_members,  AnotherObject_members},
    {Py_tp_methods, AnotherObject_methods},
    {0, 0},
};

PyType_Spec spec_another_object = {
    "AnotherObject", // tp_name
    // https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_basicsize
    sizeof(AnotherObject), // tp_basicsize
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    AnotherObject_slots // slots
};


#endif