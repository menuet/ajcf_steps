
add_library(bac-cs-network
    configuration.hpp
    client.cpp
    client.hpp
    server.cpp
    server.hpp
    connection.cpp
    connection.hpp
    messages.hpp
    run.hpp
    scheduler.hpp
    )

target_link_libraries(bac-cs-network PRIVATE common_settings)

target_include_directories(bac-cs-network PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")

target_link_libraries(bac-cs-network PUBLIC bac-util asio::asio)
