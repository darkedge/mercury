@echo off
call ..\config
:: call "$(SolutionDir)scripts\copydlls.bat" "$(Configuration)" "$(Platform)" "$(TargetDir)"

set Platform=%1
set Configuration=%2
set OutDir=%3

if not exist %OutDir%glfw3.dll copy ..\%GLFW_DIR%\%Platform%-Release\src\Release\glfw3.dll %OutDir%
