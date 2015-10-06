@echo off

if not exist mercury-x64 mkdir mercury-x64

pushd mercury-x64
set CommonCompilerFlags=-nologo -Od -I.. -I..\glfw-3.1.1\include -MTd -fp:fast -fp:except- -Gm- -GR- -EHa- -Oi -W4 -WX -wd4201 -wd4100 -wd4189 -wd4505 -wd4127
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib glfw3dll.lib opengl32.lib -libpath:..\glfw-x64\src\Release

cl %CommonCompilerFlags% ..\mercury.cpp /link %CommonLinkerFlags%
popd
