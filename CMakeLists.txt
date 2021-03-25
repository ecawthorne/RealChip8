# CMakeList.txt : CMake project for RealChip8, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (RealChip8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

add_executable (RealChip8 main.cpp
				Chip8.cpp
				Screen.cpp)
target_link_libraries(RealChip8 PRIVATE SDL2::SDL2)
