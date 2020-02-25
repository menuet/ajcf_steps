
add_executable(musify-account main.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(musify-account PRIVATE -Wall -Werror -Wno-uninitialized) # For bug hiding purpose: disable error: variable 'size' is uninitialized when used here [-Werror,-Wuninitialized]
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(musify-account PRIVATE /wd4700) # For bug hiding purpose: disable warning C4700: uninitialized local variable 'size'
endif()

target_link_libraries(musify-account PRIVATE common_settings)
