cmake_minimum_required(VERSION 3.27)
project(cpp_match)
set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_match main.cpp)

set(SOURCE_FILES main.cpp)

# METALANG99 ==========================================================================================================
include(FetchContent)
FetchContent_Declare(
        metalang99
        URL https://github.com/Hirrolot/metalang99/archive/refs/tags/v1.13.3.tar.gz
)
FetchContent_MakeAvailable(metalang99)
target_link_libraries(cpp_match metalang99)
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(cpp_match PRIVATE -fmacro-backtrace-limit=1)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(cpp_match PRIVATE -ftrack-macro-expansion=0)
endif ()
