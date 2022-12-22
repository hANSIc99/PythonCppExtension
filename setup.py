from setuptools import setup, Extension

MyModule = Extension(
                    'MyModule',
                    sources = ['my_py_module.cpp', 'my_class_py_type.cpp'],
                    extra_compile_args=['-std=c++17']
                    )

setup(ext_modules = [MyModule])