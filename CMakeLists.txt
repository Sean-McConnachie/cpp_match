cmake_minimum_required(VERSION 3.11)
project(smh LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# METALANG99 ==========================================================================================================
include(FetchContent)
FetchContent_Declare(
        metalang99
        URL https://github.com/Hirrolot/metalang99/archive/refs/tags/v1.13.3.tar.gz
)
FetchContent_MakeAvailable(metalang99)
target_link_libraries(${PROJECT_NAME} INTERFACE metalang99)
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} INTERFACE -fmacro-backtrace-limit=1)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} INTERFACE -ftrack-macro-expansion=0)
endif ()

# JSON ==================================================================================================================
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json)
