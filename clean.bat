@echo off
pushd x64-Debug
if exist *.exe del *.exe
if exist *.exp del *.exp
if exist *.lib del *.lib
if exist *.obj del *.obj
if exist *.pdb del *.pdb
if exist .vs rmdir /s /q .vs
popd
