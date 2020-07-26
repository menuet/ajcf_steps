
add_executable(bac-cs-server
    server_main.cpp
    server_app.cpp
    server_app.hpp
    )

target_link_libraries(bac-cs-server PRIVATE common_settings bac-cs-domain bac-cs-network)
