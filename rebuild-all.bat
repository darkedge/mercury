@echo off
msbuild /t:Rebuild /v:m /p:Platform=x86;Configuration="Debug" vs2015\mercury.sln
msbuild /t:Rebuild /v:m /p:Platform=x86;Configuration="Release" vs2015\mercury.sln
msbuild /t:Rebuild /v:m /p:Platform=x64;Configuration="Debug" vs2015\mercury.sln
msbuild /t:Rebuild /v:m /p:Platform=x64;Configuration="Release" vs2015\mercury.sln
