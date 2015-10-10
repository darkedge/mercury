@echo off
msbuild /t:Clean /v:m /p:Platform=x86;Configuration="Debug" vs2015\mercury.sln
msbuild /t:Clean /v:m /p:Platform=x86;Configuration="Release" vs2015\mercury.sln
msbuild /t:Clean /v:m /p:Platform=x64;Configuration="Debug" vs2015\mercury.sln
msbuild /t:Clean /v:m /p:Platform=x64;Configuration="Release" vs2015\mercury.sln
