@echo off
msbuild /t:Rebuild /v:m /p:Platform=x64;Configuration="Debug" vs2015\mercury.sln
