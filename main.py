import MyModule, gc, sys
from inspect import getmembers

print(getmembers(MyModule))

if(True):
    result = MyModule.division(4, 2)
    if result == 2:
        print("Sucessfull")
    else:
        print("Error, Result: {}".format(result))

print("\n\n")
x = MyModule.init()
print(getmembers(x))
