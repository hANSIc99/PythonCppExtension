#include "my_class_py_type.h"


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
    
    ((MyClassObject *)self)->m_value = 0;
    
    MyClassObject* m = reinterpret_cast<MyClassObject*>(self);
    m->m_myclass = (MyClass*)PyObject_Malloc(sizeof(MyClass));
    new (m->m_myclass) MyClass();

    return 0;
}

void MyClass_Dealloc(MyClassObject *self){
    std::cout << "MyClass_dealloc() called!" << std::endl;
    PyTypeObject *tp = Py_TYPE(self);
    // free references and buffers here
    MyClassObject* m = reinterpret_cast<MyClassObject*>(self);
    m->m_myclass->~MyClass();
    PyObject_Free(m->m_myclass);

    tp->tp_free(self);
    Py_DECREF(tp);
};