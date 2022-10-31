#include "my_class_py_type.h"
#include <exception>


PyObject *MyClass_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    std::cout << "MtClass_new() called!" << std::endl;

    MyClassObject *self;
    self = (MyClassObject*) type->tp_alloc(type, 0);
    if(self != NULL){ // -> allocation successfull
        // assign initial values
        self->m_value   = 0;
        self->m_myclass = NULL; 
    }
    return (PyObject*) self;
}

int MyClass_init(PyObject *self, PyObject *args, PyObject *kwds){
    
    std::cout << "MyClass_init() called!" << std::endl;
    MyClassObject* m = (MyClassObject*)self;

    m->m_value = 123
    m->m_myclass = (MyClass*)PyObject_Malloc(sizeof(MyClass));

    if(!m->m_myclass){
        PyErr_SetString(PyExc_RuntimeError, "Memory allocation failed");
        return -1;
    }

    try {
        new (m->m_myclass) MyClass();
    } catch (const std::exception& ex) {
        PyObject_Free(m->m_myclass);
        m->m_myclass = NULL;
        m->m_value   = 0;
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        return -1;
    } catch(...) {
        PyObject_Free(m->m_myclass);
        m->m_myclass = NULL;
        m->m_value   = 0;
        PyErr_SetString(PyExc_RuntimeError, "Initialization failed");
        return -1;
    }

    return 0;
}

void MyClass_dealloc(MyClassObject *self){
    std::cout << "MyClass_dealloc() called!" << std::endl;
    PyTypeObject *tp = Py_TYPE(self);

    MyClassObject* m = reinterpret_cast<MyClassObject*>(self);

    if(m->m_myclass){
        m->m_myclass->~MyClass();
        PyObject_Free(m->m_myclass);
    }

    tp->tp_free(self);
    Py_DECREF(tp);
};

PyObject* MyClass_addOne(PyObject *self, PyObject *args){
    assert(self);

    MyClassObject* _self = reinterpret_cast<MyClassObject*>(self);
    unsigned long val = _self->m_myclass->addOne();
    return PyLong_FromUnsignedLong(val);
}