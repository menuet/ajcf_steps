
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(testproj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(calcstatic STATIC
    calc.cpp
    calcsuper.cpp
    )

add_library(calcdyn SHARED
    calc.cpp
    calcsuper.cpp
    )

add_executable(exectest
    test.cpp
    )

target_link_libraries(exectest PRIVATE calcdyn)
