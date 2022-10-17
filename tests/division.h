#ifndef DIVISION_H
#define DIVISION_H

#include <Python.h>
#include <iostream>

void test_division(PyObject *pFunc){

    if(pFunc == NULL) return;

    PyObject *pResult = PyObject_CallObject(pFunc, Py_BuildValue("ll", 44, 2));
    if (!pResult) {
        std::cerr << "Cannot call division with two integers" << std::endl;
    } else {
        long result = 0;
        // https://docs.python.org/3/c-api/long.html
        result = PyLong_AsLong(pResult);
        //PyArg_ParseTuple(pResult, "l", &result);
        std::cout << "The result is: " << result << std::endl;
    }

}
#endif
