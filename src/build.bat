@echo off

set CCFLAGS=-std:c++17 -Od -nologo -Z7 -D_CRT_SECURE_NO_WARNINGS
REM Multithreaded_MSVCRT_DLL, Runtime Type, Exception Handling, Generate Intrinsic Functions
set CCFLAGS=-MD -GR- -EHa- -Oi %CCFLAGS%
set CCFLAGS=-W4 -wd4100 -wd4505 -wd4201 -wd4530 %CCFLAGS%
set CCFLAGS=-D__INTERNAL=0 %CCFLAGS%

set CLFLAGS=-incremental:no

if not exist ..\build mkdir ..\build
pushd ..\build

REM MAIN
cl -Fe:ember_compiler.exe ..\src\EMBER_main.cpp %CCFLAGS% -link %CLFLAGS%

REM VM
cl -Fe:ember_vm.exe ..\src\vm\VM_main.cpp %CCFLAGS% -link %CLFLAGS%

popd
