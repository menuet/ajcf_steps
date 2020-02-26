
cmake_minimum_required (VERSION 3.12 FATAL_ERROR)

cmake_policy(SET CMP0079 NEW)

enable_testing()

project(ajcf_steps LANGUAGES CXX)

option(AJCF_ENABLE_VCPKG "Enable vcpkg packages" ON)
option(AJCF_ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(AJCF_ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(AJCF_ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)
option(AJCF_ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" OFF)
option(AJCF_ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
option(AJCF_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" FALSE)
option(AJCF_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" FALSE)
option(AJCF_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" FALSE)
option(AJCF_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" FALSE)

include(scripts/cmake/common.cmake)
include(scripts/cmake/static_analysis.cmake)
include(scripts/cmake/sanitizers.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (AJCF_ENABLE_VCPKG)
    message(STATUS "AJCF - Using vcpg packages")
    find_package(fmt CONFIG REQUIRED)
    find_package(Catch2 CONFIG REQUIRED)
    find_package(date CONFIG REQUIRED)
    find_package(tl-expected CONFIG REQUIRED)
    find_package(Microsoft.GSL CONFIG REQUIRED)
else()
    message(STATUS "AJCF - Using internal third-parties (not recommended)")
    add_subdirectory(thirdparties_if_not_vcpkg)
endif()

add_library(common_settings INTERFACE)
ajcf_set_common_settings(common_settings)
ajcf_set_common_warnings(common_settings)
ajcf_enable_sanitizers(common_settings)

add_subdirectory(src/quickcheat)
add_subdirectory(src/quickstart)
