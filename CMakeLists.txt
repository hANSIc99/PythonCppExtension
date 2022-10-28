cmake_minimum_required(VERSION 3.14)
project(PythonCppExtension)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(PythonInterp 3.6 REQUIRED)

# This goes after, since it uses PythonInterp as hint
find_package(PythonLibs 3.6 REQUIRED)

# This comes to hand if we also need to use the NumPy C API
#exec_program(${PYTHON_EXECUTABLE}
#             ARGS "-c \"import numpy; print(numpy.get_include())\""
#             OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
#             RETURN_VALUE NUMPY_NOT_FOUND
#            )
#if(NUMPY_NOT_FOUND)
#    message(FATAL_ERROR "NumPy headers not found")
#endif()
set(SRCS "my_py_module.cpp" "my_class_py_type.cpp")
add_library(MyModule SHARED ${SRCS})
add_executable(PyDebug "pydbg.cpp")

find_package(PythonLibs REQUIRED)
#include_directories(${PYTHON_INCLUDE_DIRS} "tests/")
target_link_libraries(PyDebug ${PYTHON_LIBRARIES})

# das wird benötigt
set_target_properties(
    MyModule
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "MyModule"
        LINKER_LANGUAGE C # Egal ob hier C oder CXX steht
        LINK_LIBRARIES "${PYTHON_LIBRARIES}"
        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
    )
set_target_properties(
    PyDebug
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "PyDebug"
        LINKER_LANGUAGE C # Egal ob hier C oder CXX steht
        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
    )
