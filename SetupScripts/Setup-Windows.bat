@echo off

pushd ..
vendor\bin\premake\Windows\premake5.exe --file=build.lua vs2022
popd
pause