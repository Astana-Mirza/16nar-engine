cmake_minimum_required(VERSION 3.14)

project(example-game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENGINE_DIR ../install/x64-Debug)


add_executable(example-game
    ${SRC_DIR}/main.cpp
)
add_library(example-level SHARED ${SRC_DIR}/example_level.cpp)

target_include_directories(example-game PRIVATE ${ENGINE_DIR}/include)
target_link_directories(example-game PRIVATE ${ENGINE_DIR}/lib)
target_link_libraries(example-game PRIVATE engine)

target_include_directories(example-level PRIVATE ${ENGINE_DIR}/include)
target_link_directories(example-level PRIVATE ${ENGINE_DIR}/lib)
target_link_libraries(example-level PRIVATE engine)

if (WIN32)
    install(FILES ${ENGINE_DIR}/bin/engine.dll DESTINATION ${PROJECT_BINARY_DIR})
    if (USE_SFML)
        if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
            install(FILES ${ENGINE_DIR}/bin/sfml-system-d-2.dll DESTINATION ${PROJECT_BINARY_DIR})
            install(FILES ${ENGINE_DIR}/bin/sfml-window-d-2.dll DESTINATION ${PROJECT_BINARY_DIR})
            install(FILES ${ENGINE_DIR}/bin/sfml-graphics-d-2.dll DESTINATION ${PROJECT_BINARY_DIR})
            install(FILES ${ENGINE_DIR}/bin/sfml-audio-d-2.dll DESTINATION ${PROJECT_BINARY_DIR})
        else()
            install(FILES ${ENGINE_DIR}/bin/sfml-system-2.dll DESTINATION ${PROJECT_BINARY_DIR})
            install(FILES ${ENGINE_DIR}/bin/sfml-window-2.dll DESTINATION ${PROJECT_BINARY_DIR})
            install(FILES ${ENGINE_DIR}/bin/sfml-graphics-2.dll DESTINATION ${PROJECT_BINARY_DIR})
            install(FILES ${ENGINE_DIR}/bin/sfml-audio-2.dll DESTINATION ${PROJECT_BINARY_DIR})
        endif()
    endif()
endif()