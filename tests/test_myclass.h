#ifndef TEST_MYCLASS_H
#define TEST_MYCLASS_H

#include <Python.h>
#include <iostream>
#include "get_func.h"
#include "object.h"

void test_example(PyObject *pFunc){

    if(pFunc == NULL) return;

    PyObject *pMyClass = PyObject_CallObject(pFunc, NULL);
    if (!pMyClass) {
        std::cerr << "Cannot call example()" << std::endl;
    } else {
        std::cout << "Example() called" << std::endl;
    }

}

void test_myclass(PyObject *pFunc){

    if(pFunc == NULL) return;

    PyObject *pMyClass = PyObject_CallObject(pFunc, NULL);
    if (!pMyClass) {
        std::cerr << "Cannot call init()" << std::endl;
        return;
    } else {
        std::cout << "Init called" << std::endl;
    }


    test_example(get_func(pMyClass, "example"));
}



#endif