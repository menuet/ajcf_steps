
add_library(musify-holdall
    input_output.cpp
    input_output.hpp
    )

target_link_libraries(musify-holdall PRIVATE common_settings)

target_include_directories(musify-holdall INTERFACE ..)
