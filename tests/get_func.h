#ifndef GET_FUNC_H
#define GET_FUNC_H

#include <Python.h>
#include <iostream>

PyObject* get_func(PyObject *pmodule, const char* fName){

    PyObject* pFunc = PyObject_GetAttrString(pmodule, fName);
    if (!pFunc) {
        std::cerr << "Cannot find function " << fName << std::endl;
        return NULL;
    }

    if (!PyCallable_Check(pFunc)) {
        std::cerr << "Function " << fName << " is not calable" << std::endl;
        return NULL;
    }
    return pFunc;
}
#endif