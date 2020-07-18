
add_library(musify-holdall
    input_output.cpp
    input_output.hpp
    input_output.impl.hpp
    database.cpp
    database.hpp
    database.impl.cpp
    database.impl.hpp
    strong_types.cpp
    strong_types.hpp
    musical_thing.cpp
    musical_thing.hpp
    musical_base.cpp
    musical_base.hpp
    musical_things.cpp
    musical_things.hpp
    musical_factory.cpp
    musical_factory.hpp
    )

target_link_libraries(musify-holdall PRIVATE common_settings fmt::fmt PUBLIC date::date)

target_compile_definitions(musify-holdall PUBLIC TRY_RISKY_FIX)

target_include_directories(musify-holdall INTERFACE ..)

add_executable(musify-holdall-ut
    input_output.ut.cpp
    database.ut.cpp
    strong_types.ut.cpp
    )

target_link_libraries(musify-holdall-ut PRIVATE common_settings Catch2::Catch2 musify-holdall)

add_test(NAME musify-holdall-ut COMMAND musify-holdall-ut)

add_custom_command(
    TARGET musify-holdall-ut
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/database.txt" "$<TARGET_FILE_DIR:musify-holdall-ut>"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/database.txt"
    )
