# PythonCppExtension

## Build

To configure the build, CMake is required

### Using Python

```bash
python3 setup.py build
```

### Invoke CMake manually

```bash
cmake -B build
cmake --build build
```

## Debugging

### GDB

Start a GDB session
```bash
./gdb.sh
```
### C++ application

Alternatively, debug the application `pydbg.cpp` which embeds the Python interpreter and imports the module.

## Python

Start an interactive Python session:
```bash
./init.sh
```