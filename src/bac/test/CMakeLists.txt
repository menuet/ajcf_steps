
add_executable(bac-test
    actor_test.cpp
    board_test.cpp
    codebreaker_test.cpp
    codemaker_test.cpp
    input_test.cpp
    menu_test.cpp
    logic_test.cpp
    options_test.cpp
    solver_test.cpp
    random_test.cpp
    test_main.cpp
    )

add_test(NAME bac-test COMMAND bac-test)

target_link_libraries(bac-test PRIVATE common_settings Catch2::Catch2 bac-domain)
