#include "myclass.h"
#include <iostream>

MyClass::MyClass()
{
    std::cout << __func__ << std::endl;
}

MyClass::~MyClass(){
    std::cout << __func__ << std::endl;
}
