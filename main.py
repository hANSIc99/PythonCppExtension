import gc, sys
from MyModule import MyClass
from inspect import getmembers

mc = MyClass()

for attr in getmembers(mc):
    print(attr)
