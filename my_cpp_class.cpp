#include "my_cpp_class.h"
#include <iostream>

MyClass::MyClass()
{
    std::cout << "MyClass::MyClass() called!" << std::endl;
}

MyClass::~MyClass(){
    std::cout << "MyClass::~MyClass() called" << std::endl;
}
