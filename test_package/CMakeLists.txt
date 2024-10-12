cmake_minimum_required(VERSION 3.15)

project(test_package LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(16nar REQUIRED)

add_executable("test_package"
    "${PROJECT_SOURCE_DIR}/test_package.cpp"
)

target_link_libraries("test_package" "16nar::16nar_constructor2d" "16nar::16nar_render_gl")
