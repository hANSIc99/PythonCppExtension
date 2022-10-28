#ifndef MYCLASS_H
#define MYCLASS_H

#include <iostream>

class MyClass
{
public:
    MyClass() : m_cnt(0) {std::cout << "MyClass::MyClass() called!" << std::endl;};

    virtual ~MyClass() {std::cout << "MyClass::~MyClass() called" << std::endl;};

    unsigned long addOne() { return ++m_cnt;};

private:
    unsigned long m_cnt;
};

#endif // MYCLASS_H
