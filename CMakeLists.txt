cmake_minimum_required(VERSION 3.14)
project(PythonCppExtension)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(SRCS "my_py_module.cpp" "my_class_py_type.cpp")
add_library(MyModule SHARED ${SRCS})
add_executable(PyDebug "pydbg.cpp")

find_package(PythonLibs REQUIRED)
target_link_libraries(PyDebug ${PYTHON_LIBRARIES})

set_target_properties(
    MyModule
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "MyModule"
        LINKER_LANGUAGE C
        LINK_LIBRARIES "${PYTHON_LIBRARIES}"
        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
        # COMPILE_DEFINITIONS "Py_LIMITED_API=0x03090000" # (broke in default installation)
    )
set_target_properties(
    PyDebug
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "PyDebug"
        LINKER_LANGUAGE C
        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
    )
