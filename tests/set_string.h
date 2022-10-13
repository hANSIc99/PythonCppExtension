#ifndef SET_STRING_H
#define SET_STRING_H

#include <Python.h>
#include <iostream>

void test_setString(PyObject *pFunc){

    if(pFunc == NULL) return;

    PyObject *argString = Py_BuildValue("s", "Before string");

    int test = PyUnicode_Check(argString); // 1
    const char* is_unicode = test ? "yes" : "no";
    std::cout << "Object is unicode: " << is_unicode << std::endl;

    std::string sBefore(PyUnicode_AsUTF8(argString));
    std::cout << "String before: \"" << sBefore << "\"" << std::endl;

    PyObject *pResult = PyObject_CallObject(pFunc, argString);
    if (!pResult) {
        std::cerr << "Cannot call setString" << std::endl;
    } else {
        std::cerr << "setString() called" << std::endl;
    }
}
#endif
