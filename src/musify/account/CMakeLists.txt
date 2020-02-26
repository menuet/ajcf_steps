
add_executable(musify-account
    main.cpp
    accounts.cpp
    accounts.hpp
    )

target_link_libraries(musify-account PRIVATE common_settings fmt::fmt musify-holdall)
