@echo off
pushd vs2015
cl /analyze:only /nologo /I"..\src" /I"..\glfw-3.1.1\include" ..\src\mercury.cpp
popd
