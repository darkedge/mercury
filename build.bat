@echo off

if not exist mercury-x64 mkdir mercury-x64

:: x64-debug
pushd mercury-x64
set CommonCompilerFlags=/nologo /Od /I".." /I"..\glfw-3.1.1\include" /MTd /fp:fast /fp:except- /Gm- /GR- /EHa- /Oi /W4 /WX /wd4201 /wd4100 /wd4189 /wd4505 /wd4127
set CommonLinkerFlags=/INCREMENTAL:NO /OPT:REF "user32.lib" "gdi32.lib" "glfw3dll.lib" "opengl32.lib" /LIBPATH:"..\glfw-x64\src\Release"

cl %CommonCompilerFlags% ..\mercury.cpp /link %CommonLinkerFlags%
popd
