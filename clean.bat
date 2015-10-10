@echo off
msbuild /t:Clean /v:m /p:Platform=x64;Configuration="Debug" vs2015\mercury.sln
