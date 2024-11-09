@echo off
pushd ..\data
if exist output.txt del output.txt
..\build\ember_compiler.exe >> output.txt
output.txt
popd
