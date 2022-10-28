#!/bin/sh
cmake -B build/ -D CMAKE_BUILD_TYPE=Debug
cmake --build build
export PYTHONPATH="build/"
#gdb -x gdbinit --tui --args python3 -i main.py
gdb -x gdbinit --args python3 -i main.py
