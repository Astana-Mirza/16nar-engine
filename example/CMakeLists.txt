cmake_minimum_required(VERSION 3.15)

project(example-game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENGINE_DIR ../engine)
set(ENGINE_BUILD_DIR ../out/build/x64-Debug/engine)
set(ENGINE_DEPS_DIR ../out/build/x64-Debug/_deps)

add_executable(example-game
    ${SRC_DIR}/main.cpp
)
#add_library(example-level SHARED ${SRC_DIR}/example_level.cpp)

target_include_directories(example-game PRIVATE ${ENGINE_DIR}/include ${ENGINE_BUILD_DIR} ${ENGINE_DEPS_DIR}/glm-src ${ENGINE_DEPS_DIR}/glfw-src)
target_link_directories(example-game PRIVATE ${ENGINE_DIR}/lib)
target_link_libraries(example-game PRIVATE engine)

#target_include_directories(example-level PRIVATE ${ENGINE_DIR}/include)
#target_link_directories(example-level PRIVATE ${ENGINE_DIR}/lib)
#target_link_libraries(example-level PRIVATE engine)

if (WIN32)
    install(FILES ${ENGINE_DIR}/bin/engine.dll DESTINATION ${PROJECT_BINARY_DIR})
endif()