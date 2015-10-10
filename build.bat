@echo off
msbuild /v:m /p:Platform=x64;Configuration="Debug" vs2015\mercury.sln
