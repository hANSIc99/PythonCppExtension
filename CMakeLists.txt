cmake_minimum_required(VERSION 3.14)
project(mymath)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
set(SRCS "my_module.cpp" "myclass.cpp")
add_library(mymath SHARED ${SRCS})
add_executable(PyDebug "pydbg.cpp")

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS} "tests/")
target_link_libraries(PyDebug ${PYTHON_LIBRARIES})

# das wird benötigt
set_target_properties(
    mymath
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "MyModule"
        LINKER_LANGUAGE C # Egal ob hier C oder CXX steht
    )
set_target_properties(
    PyDebug
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "PyDebug"
        LINKER_LANGUAGE C # Egal ob hier C oder CXX steht
    )
