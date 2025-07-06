@echo off
setlocal enabledelayedexpansion

:: Get root dir where Build.bat is
set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "INCLUDE=%ROOT%include"
set "OUT=%ROOT%main.exe"

:: Compile BigInteger and main.cpp
echo Compiling project...
g++ "%SRC%\BigInteger.cpp" "%SRC%\main.cpp" -I"%INCLUDE%" -o "%OUT%"

:: Stop if compile failed
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)

:: Run the compiled executable
echo.
echo Running main.exe:
echo ====================
"%OUT%"
echo ====================

:: Wait so you can see the output
pause
endlocal
