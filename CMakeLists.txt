# cmake_minimum_required(VERSION <specify CMake version here>)
project(TuringSimulator C)

set(CMAKE_C_STANDARD 99)

add_executable(TuringSimulator input.c input.h main.c output.c step.c)