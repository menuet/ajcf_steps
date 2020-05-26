
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(testproj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(exectest
    calc.cpp
    calcsuper.cpp
    test.cpp
    )

