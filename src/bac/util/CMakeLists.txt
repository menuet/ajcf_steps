
add_library(bac-util
    input.cpp
    input.hpp
    random.hpp
    actor.hpp
    menu.hpp
    )

target_link_libraries(bac-util PRIVATE common_settings)

target_include_directories(bac-util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")

