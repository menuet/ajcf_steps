
function(ajcf_enable_sanitizers base_target_name)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

        if(AJCF_ENABLE_COVERAGE)
            message(STATUS "AJCF - Enabling code coverage")
            target_compile_options("${base_target_name}" INTERFACE --coverage -O0 -g)
            target_link_libraries("${base_target_name}" INTERFACE --coverage)
        endif()

        set(SANITIZERS "")

        if(AJCF_ENABLE_SANITIZER_ADDRESS)
            message(STATUS "AJCF - Enabling address sanitizer")
            list(APPEND SANITIZERS "address")
        endif()

        if(AJCF_ENABLE_SANITIZER_MEMORY)
            message(STATUS "AJCF - Enabling memory sanitizer")
            list(APPEND SANITIZERS "memory")
        endif()

        if(AJCF_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
            message(STATUS "AJCF - Enabling ub sanitizer")
            list(APPEND SANITIZERS "undefined")
        endif()

        if(AJCF_ENABLE_SANITIZER_THREAD)
            message(STATUS "AJCF - Enabling thread sanitizer")
            list(APPEND SANITIZERS "thread")
        endif()

        list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

    endif()

    if(LIST_OF_SANITIZERS AND NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
        target_compile_options("${base_target_name}" INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
        target_link_libraries("${base_target_name}" INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
    endif()

endfunction()
