:: Setup GLFW
cmake -S Raytracy\vendor\glfw -B Raytracy\vendor\glfw\build

call vendor\bin\premake\premake5.exe vs2022
PAUSE