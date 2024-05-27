:: Setup GLFW
cmake -S Raytracy\vendor\glfw -B Raytracy\vendor\glfw\build

:: Setup GoogleTest
cmake -S Test\vendor\googletest -B Test\vendor\googletest\build

call vendor\bin\premake\premake5.exe vs2022
PAUSE