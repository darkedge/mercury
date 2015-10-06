@echo off
:: 32-bit shared build
if not exist glfw-Win32 mkdir glfw-Win32
pushd glfw-Win32
cmake -G"Visual Studio 14 2015" -DBUILD_SHARED_LIBS=ON ..\glfw-3.1.1
msbuild /v:m /p:Configuration="Release";OutDir=..\..\mercury-Win32\ src\glfw.vcxproj
popd

:: 64-bit shared build
if not exist glfw-x64 mkdir glfw-x64
pushd glfw-x64
cmake -G"Visual Studio 14 2015 Win64" -DBUILD_SHARED_LIBS=ON ..\glfw-3.1.1
msbuild /v:m /p:Configuration="Release";OutDir=..\..\mercury-x64\ src\glfw.vcxproj
popd
