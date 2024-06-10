cmake_minimum_required(VERSION 3.15)
include(FetchContent)

project(scene-compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY "https://github.com/nlohmann/json.git"
    GIT_TAG        "v3.11.2"
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(scene-compiler
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/compiler.cpp
)

target_include_directories(scene-compiler PRIVATE
                           ${INCLUDE_DIR}
                           ${COMMON_DIR}
                           "${FETCHCONTENT_BASE_DIR}/nlohmann_json-src/single_include")
