# cmake_minimum_required(VERSION <specify CMake version here>)
project(TuringSimulator C)

set(CMAKE_C_STANDARD 99)

add_executable(TuringSimulator parser.c parser.h main.c simulator.h simulator.c main.h)