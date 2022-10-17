import MyModule, gc, sys
from inspect import getmembers

print(getmembers(MyModule))

print("\n\n")
x = MyModule.init()
print(getmembers(x))
