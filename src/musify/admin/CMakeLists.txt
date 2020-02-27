
add_executable(musify-admin
    main.cpp
    admin.hpp
    artists.cpp
    artists.hpp
    )

target_link_libraries(musify-admin PRIVATE common_settings musify-holdall)
