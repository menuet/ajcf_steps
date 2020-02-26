
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    if(MSVC)
        message(STATUS "AJCF - Using compiler clang-cl")
    else()
        message(STATUS "AJCF - Using compiler clang")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "AJCF - Using compiler gcc")
elseif(MSVC)
    message(STATUS "AJCF - Using compiler msvc")
else()
    message(SEND_ERROR "Unsupported compiler for now")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "AJCF - Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)
endif()

if(AJCF_ENABLE_IPO)
    message(STATUS "AJCF - Enabling IPO")
    include(CheckIPOSupported)
        check_ipo_supported(RESULT result OUTPUT output)
    if(result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(SEND_ERROR "IPO is not supported: ${output}")
    endif()
endif()

function(ajcf_set_common_settings base_target_name)

    target_compile_features("${base_target_name}" INTERFACE cxx_std_17)

    target_link_libraries("${base_target_name}" INTERFACE Threads::Threads)

endfunction()

function(ajcf_set_common_warnings base_target_name)

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        target_compile_options("${base_target_name}" INTERFACE
            -Werror
            -Wall
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-missing-prototypes
            -Wno-c++2a-compat
            -Wno-documentation-unknown-command
            -Wno-inconsistent-missing-destructor-override
            -Wno-signed-enum-bitfield
            -Wno-deprecated
            -Wno-undefined-func-template
            )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options("${base_target_name}" INTERFACE -Werror -Wall)
    elseif(MSVC)
        target_compile_options("${base_target_name}" INTERFACE /W4 /WX)
        target_compile_options("${base_target_name}" INTERFACE /wd4275) # Disable warning in fmt: warning C4275: non dll-interface class 'std::runtime_error' used as base for dll-interface class 'fmt::v6::format_error'
    endif()

    if(MSVC)
        target_compile_definitions("${base_target_name}" INTERFACE _CRT_SECURE_NO_WARNINGS)
    endif()

endfunction()
