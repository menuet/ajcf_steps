
add_executable(musify-browse
    browse_main.cpp
    )

target_link_libraries(musify-browse PRIVATE common_settings musify-holdall)
