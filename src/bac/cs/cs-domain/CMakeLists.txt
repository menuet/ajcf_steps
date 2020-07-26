
add_library(bac-cs-domain
    protocol.cpp
    protocol.hpp
    actors.cpp
    actors.hpp
    )

target_link_libraries(bac-cs-domain PRIVATE common_settings)

target_include_directories(bac-cs-domain PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")

target_link_libraries(bac-cs-domain PUBLIC bac-domain bac-util nlohmann_json::nlohmann_json)
