import mymath
from inspect import getmembers

print(getmembers(mymath))

if(True):
    result = mymath.division(4, 2)
    if result == 2:
        print("Sucessfull")
    else:
        print("Error, Result: {}".format(result))
