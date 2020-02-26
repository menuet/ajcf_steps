
add_executable(quickstart main.cpp)

add_test(NAME quickstart COMMAND quickstart)

target_link_libraries(quickcheat PRIVATE common_settings)
