cmake_minimum_required(VERSION 3.14)

project(16nar-engine LANGUAGES CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/scene-compiler)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)