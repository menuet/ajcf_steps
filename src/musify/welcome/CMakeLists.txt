
add_executable(musify-welcome main.cpp)

add_test(NAME musify-welcome COMMAND musify-welcome)

target_link_libraries(musify-welcome PRIVATE common_settings)
