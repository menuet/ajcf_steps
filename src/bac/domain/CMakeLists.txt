
add_library(bac-domain
    codebreaker.cpp
    codebreaker.hpp
    codemaker.cpp
    codemaker.hpp
    board.cpp
    board.hpp
    game.cpp
    game.hpp
    logic.cpp
    logic.hpp
    options.cpp
    options.hpp
    solver.cpp
    solver.hpp
    )

target_compile_definitions(bac-domain PRIVATE PLAY_WITH_ACTORS=0)

target_link_libraries(bac-domain PRIVATE common_settings PUBLIC bac-util)
