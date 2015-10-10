@echo off
call ..\config

set Platform=%1
set Configuration=%2
set OutDir=%3

:: Copy glfw3.dll
if not exist %OutDir%glfw3.dll copy ..\%GLFW_DIR%\%Platform%-Release\src\Release\glfw3.dll %OutDir%
:: Create symbolic link to assets folder
:: if not exist %OutDir%assets mklink /d \%OutDir%assets \..\assets
