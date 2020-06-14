
add_executable(bac-game
    game_main.cpp
    )

target_link_libraries(bac-game PRIVATE common_settings bac-domain)
