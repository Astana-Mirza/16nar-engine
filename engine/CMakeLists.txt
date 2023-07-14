cmake_minimum_required(VERSION 3.14)
include(FetchContent)

macro(narengine_set_option var default opt_type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${opt_type} ${docstring} FORCE)
endmacro()

macro(narengine_git_dependency dep_name git_repo git_tag incl_dir)
    FetchContent_Declare(
        ${dep_name}
        GIT_REPOSITORY "${git_repo}"
        GIT_TAG        "${git_tag}"
    )
    set(${dep_name}_INCLUDE_DIR "${FETCHCONTENT_BASE_DIR}/${incl_dir}")
    FetchContent_MakeAvailable(${dep_name})
endmacro()


# Build settings
project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(BUILD_SHARED_LIBS ON)

set(NAME 16nar)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common)

narengine_git_dependency(GLM https://github.com/g-truc/glm bf71a83 glm-src)

narengine_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")
narengine_set_option(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" STRING "Choose the install directory")
narengine_set_option(NARENGINE_RENDER_OPENGL ON BOOL "Use OpenGL for rendering (uses GLM for math)")
narengine_set_option(NARENGINE_RENDER_VULKAN OFF BOOL "Use Vulkan for rendering (uses GLM for math)") # not implemented
narengine_set_option(NARENGINE_BUILD_TESTS ON BOOL "Build tests for 16nar engine")

if (WIN32)
    set(NARENGINE_WIN_EXPORT ON)
    set(ALL_COMPILER_DEFS ${ALL_COMPILER_DEFS} BUILDING_DLL)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()


# Main engine build
add_library(engine
    #${SRC_DIR}/common/game.cpp
    #${SRC_DIR}/abstract/signallable.cpp
    #${SRC_DIR}/constructor/abstract/drawable_node.cpp
    #${SRC_DIR}/constructor/abstract/node.cpp
    #${SRC_DIR}/constructor/states/scene_state.cpp
    #${SRC_DIR}/constructor/tiles/tile.cpp
    #${SRC_DIR}/constructor/animations/animation.cpp
    #${SRC_DIR}/constructor/node_2d.cpp
    #${SRC_DIR}/constructor/sprite_node.cpp
    #${SRC_DIR}/constructor/sound_node.cpp
    #${SRC_DIR}/constructor/text_node.cpp
    #${SRC_DIR}/constructor/world_node.cpp
    #${SRC_DIR}/constructor/tilemap_node.cpp
    #${SRC_DIR}/common/render/quadrant.cpp
    #${SRC_DIR}/common/render/qtree_render_system.cpp
    #${SRC_DIR}/common/render/view.cpp
    #${SRC_DIR}/common/render/frame_render_device.cpp
    #${SRC_DIR}/common/system/dynamic_lib.cpp
    #${SRC_DIR}/common/system/file_scene_reader.cpp
    #${SRC_DIR}/common/system/event_manager.cpp
    #${SRC_DIR}/common/system/listener.cpp
    #${SRC_DIR}/common/profiles/single_thread_profile.cpp
    ${SRC_DIR}/common/math/math_functions.cpp
    ${SRC_DIR}/common/math/transform_matrix.cpp
)

configure_file(${INCLUDE_DIR}/${NAME}config.h.in ${NAME}/${NAME}config.h)

set(ALL_INCLUDE_DIRS
     "${INCLUDE_DIR}"
     "${CMAKE_CURRENT_BINARY_DIR}"
     "${COMMON_DIR}"
     "${GLM_INCLUDE_DIR}"
)

set(ALL_LINK_DIRS
     "${CMAKE_CURRENT_BINARY_DIR}"
)

target_include_directories(engine PRIVATE ${ALL_INCLUDE_DIRS})
target_compile_definitions(engine PRIVATE ${ALL_COMPILER_DEFS})
target_link_directories(engine PRIVATE ${ALL_LINK_DIRS})
target_link_libraries(engine PRIVATE ${ALL_LINK_LIBS})


# Testing
if (${NARENGINE_BUILD_TESTS})
    enable_testing()
    narengine_git_dependency(CATCH2 https://github.com/catchorg/Catch2.git v3.0.1 catch2-src/src)

    add_executable(math_test
        ${SRC_DIR}/common/math/test/transform_matrix_test.cpp
        ${SRC_DIR}/common/math/test/vec_test.cpp
        ${SRC_DIR}/common/math/test/rectangle_test.cpp
        ${SRC_DIR}/common/math/test/math_functions_test.cpp
    )
    target_include_directories(math_test PRIVATE ${ALL_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories(math_test PRIVATE ${ALL_LINK_DIRS})
    target_link_libraries(math_test PRIVATE engine Catch2::Catch2WithMain)
    add_test(NAME math_test COMMAND math_test)
endif()

# Installation
install(DIRECTORY ${INCLUDE_DIR}/${NAME} DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/${NAME}/${NAME}config.h DESTINATION include/${NAME})
install(FILES ${COMMON_DIR}/${NAME}/scene_file.h DESTINATION include/${NAME})
if (WIN32)
    install(FILES ${PROJECT_BINARY_DIR}/engine.lib DESTINATION lib)
    install(FILES ${PROJECT_BINARY_DIR}/engine.dll DESTINATION bin)
else()
    install(TARGETS engine DESTINATION lib)
endif()
