
add_executable(musify-account
    main.cpp
    input_output.cpp
    input_output.hpp
    )

target_link_libraries(musify-account PRIVATE common_settings)
