
add_executable(quickstart main.cpp)

add_test(NAME quickstart COMMAND quickstart)

target_link_libraries(quickstart PRIVATE common_settings)
