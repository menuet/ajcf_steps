
add_library(musify-holdall
    input_output.cpp
    input_output.hpp
    input_output.impl.hpp
    )

target_link_libraries(musify-holdall PRIVATE common_settings)

target_include_directories(musify-holdall INTERFACE ..)

add_executable(musify-holdall-ut input_output.ut.cpp)

target_link_libraries(musify-holdall-ut PRIVATE common_settings Catch2::Catch2 musify-holdall)

add_test(NAME musify-holdall-ut COMMAND musify-holdall-ut)
