

add_executable(bac-cs-client
    client_main.cpp
    client_app.cpp
    client_app.hpp
    )

target_link_libraries(bac-cs-client PRIVATE common_settings bac-cs-domain bac-cs-network)
