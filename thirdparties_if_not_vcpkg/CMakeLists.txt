
add_library(thirdparty_catch2 INTERFACE)
target_include_directories(thirdparty_catch2 INTERFACE catch2/inc)
add_library(Catch2::Catch2 ALIAS thirdparty_catch2)

add_library(thirdparty_fmt SHARED fmt/src/format.cc)
target_compile_definitions(thirdparty_fmt PRIVATE FMT_EXPORT INTERFACE FMT_SHARED)
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(thirdparty_fmt PUBLIC
        -Wno-nonportable-system-include-path
        -Wno-sign-conversion
        -Wno-missing-variable-declarations
        -Wno-unused-member-function
        )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(thirdparty_fmt PUBLIC
        -Wno-deprecated-declarations
        -Wno-unknown-pragmas
        )
elseif(MSVC)
    target_compile_options(thirdparty_fmt PRIVATE /wd4275) # Disable warning in fmt: warning C4275: non dll-interface class 'std::runtime_error' used as base for dll-interface class 'fmt::v6::format_error'
    target_compile_options(thirdparty_fmt PUBLIC /wd4273)
endif()
target_include_directories(thirdparty_fmt PUBLIC fmt/inc)
add_library(fmt::fmt ALIAS thirdparty_fmt)

add_library(thirdparty_date INTERFACE)
target_include_directories(thirdparty_date INTERFACE date/inc)
add_library(date::date ALIAS thirdparty_date)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    add_library(thirdparty_date_tz date/src/tz.cpp)
    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        target_compile_options(thirdparty_date_tz PUBLIC
            -Wno-nonportable-system-include-path
            -Wno-shadow-field-in-constructor
            -Wno-exit-time-destructors
            -Wno-unused-macros
            )
    endif()
    target_include_directories(thirdparty_date_tz PUBLIC date/inc)
    add_library(date::tz ALIAS thirdparty_date_tz)
else()
    add_library(thirdparty_date_tz INTERFACE)
    target_compile_definitions(thirdparty_date_tz INTERFACE DISABLED_DATE_TZ_ON_LINUX) # disable on linux because of issue with TZ database
    add_library(date::tz ALIAS thirdparty_date_tz)
endif()

add_library(thirdparty_tl INTERFACE)
target_include_directories(thirdparty_tl INTERFACE tl/inc)
add_library(tl::expected ALIAS thirdparty_tl)

add_library(thirdparty_ms_gsl INTERFACE)
target_include_directories(thirdparty_ms_gsl INTERFACE ms-gsl/inc)
add_library(Microsoft.GSL::GSL ALIAS thirdparty_ms_gsl)
