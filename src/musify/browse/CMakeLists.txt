
add_executable(musify-browse
    browse_main.cpp
    browse_app.cpp
    browse_app.hpp
    menu_options.hpp
    )

target_link_libraries(musify-browse PRIVATE common_settings musify-holdall)
