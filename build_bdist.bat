::set PYTHONPATH=%PYTHONPATH%;..\out\build\x64-Release\PyModule
:: requires https://pypa-build.readthedocs.io/ 
:: and PATH environment variable to be set to C:\Users\StephanA\AppData\Roaming\Python\Python39\Scripts
python -m build --wheel