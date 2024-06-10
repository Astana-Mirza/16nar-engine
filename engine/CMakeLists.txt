cmake_minimum_required(VERSION 3.15)
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

set(NAME 16nar)
set(NARENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(NARENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(NARENGINE_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common)

narengine_set_option(CMAKE_BUILD_TYPE Release STRING "Type of the build (Debug or Release)")
narengine_set_option(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" STRING "Install directory")
narengine_set_option(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" STRING "MSVC runtime library type")
narengine_set_option(BUILD_SHARED_LIBS ON BOOL "Build shared libraries instead of static ones")
# Wayland needs these packages: libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules; X11 needs xorg-dev
narengine_set_option(NARENGINE_WINDOW_WAYLAND OFF BOOL "Use Wayland instead of X11 (UNIX only)")
narengine_set_option(NARENGINE_RENDER_OPENGL ON BOOL "Use OpenGL for rendering (uses GLM + GLFW + glad)")
narengine_set_option(NARENGINE_RENDER_VULKAN OFF BOOL "Use Vulkan for rendering (uses GLM + GLFW)") # not implemented
narengine_set_option(NARENGINE_RENDER_OPENGL_ES OFF BOOL "Use OpenGL ES for rendering (uses GLM + GLFW + glad)") # not implemented
narengine_set_option(NARENGINE_LOG_LEVEL "9" STRING "Maximum log level of engine logging library")
narengine_set_option(NARENGINE_BUILD_TESTS ON BOOL "Build tests for 16nar engine")

# Basic settings

if (${WIN32} AND ${BUILD_SHARED_LIBS})
    set(NARENGINE_WIN_EXPORT ON)
    set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS} "NARENGINE_DLL")
endif()
set(NARENGINE_COMMON_LINK_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
set(NARENGINE_COMMON_INCLUDE_DIRS
    "${NARENGINE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${NARENGINE_COMMON_DIR}"
)
configure_file("${NARENGINE_INCLUDE_DIR}/${NAME}config.h.in" "${NAME}/${NAME}config.h")


# Logger library
set(NARENGINE_LOGGER_COMPILER_DEFS
     "NARENGINE_LOG_LEVEL=${NARENGINE_LOG_LEVEL}"
     ${NARENGINE_COMMON_COMPILER_DEFS}
)
set(NARENGINE_LOGGER_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS})
set(NARENGINE_LOGGER_SOURCES
    ${NARENGINE_SRC_DIR}/logger/logger.cpp
    ${NARENGINE_SRC_DIR}/logger/base_log_writer.cpp
    ${NARENGINE_SRC_DIR}/logger/terminal_log_writer.cpp
)
add_library(engine_logger ${NARENGINE_LOGGER_SOURCES})
target_include_directories(engine_logger PUBLIC ${NARENGINE_LOGGER_INCLUDE_DIRS})
target_compile_definitions(engine_logger PUBLIC ${NARENGINE_LOGGER_COMPILER_DEFS})


# Math library
narengine_git_dependency(GLM https://github.com/g-truc/glm "bf71a83" "glm-src")
set(NARENGINE_MATH_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS}
    "GLM_FORCE_INLINE"
    "GLM_FORCE_DEFAULT_ALIGNED_GENTYPES"
    "GLM_FORCE_INTRINSICS"
    "GLM_FORCE_SIZE_T_LENGTH"
)
set(NARENGINE_MATH_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS} "${GLM_INCLUDE_DIR}")
set(NARENGINE_MATH_SOURCES
    ${NARENGINE_SRC_DIR}/math/math_functions.cpp
    ${NARENGINE_SRC_DIR}/math/transform_matrix.cpp
)
set(NARENGINE_MATH_LINK_LIBS engine_logger)
add_library(engine_math ${NARENGINE_MATH_SOURCES})
target_include_directories(engine_math PUBLIC ${NARENGINE_MATH_INCLUDE_DIRS})
target_compile_definitions(engine_math PUBLIC ${NARENGINE_MATH_COMPILER_DEFS})
target_link_libraries(engine_math PUBLIC ${NARENGINE_MATH_LINK_LIBS})


# Main engine library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
set(GLFW_USE_WAYLAND "${NARENGINE_WINDOW_WAYLAND}")
narengine_git_dependency(GLFW https://github.com/glfw/glfw "3.3.8" "glfw-src/include")

set(NARENGINE_BASE_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS} "GLFW_INCLUDE_NONE")
if (${BUILD_SHARED_LIBS} AND ${WIN32})
    set(NARENGINE_BASE_COMPILER_DEFS ${NARENGINE_BASE_COMPILER_DEFS} "GLFW_DLL")
endif()
set(NARENGINE_BASE_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS} "${GLFW_INCLUDE_DIR}")
set(NARENGINE_BASE_LINK_LIBS engine_math glfw)
set(NARENGINE_BASE_LINK_DIRS ${NARENGINE_COMMON_LINK_DIRS})
set(NARENGINE_BASE_SOURCES
    ${NARENGINE_SRC_DIR}/game.cpp
    #${NARENGINE_SRC_DIR}/abstract/signallable.cpp
    #${NARENGINE_SRC_DIR}/constructor/abstract/drawable_node.cpp
    #${NARENGINE_SRC_DIR}/constructor/abstract/node.cpp
    #${NARENGINE_SRC_DIR}/constructor/states/scene_state.cpp
    #${NARENGINE_SRC_DIR}/constructor/tiles/tile.cpp
    #${NARENGINE_SRC_DIR}/constructor/animations/animation.cpp
    #${NARENGINE_SRC_DIR}/constructor/node_2d.cpp
    #${NARENGINE_SRC_DIR}/constructor/sprite_node.cpp
    #${NARENGINE_SRC_DIR}/constructor/sound_node.cpp
    #${NARENGINE_SRC_DIR}/constructor/text_node.cpp
    #${NARENGINE_SRC_DIR}/constructor/world_node.cpp
    #${NARENGINE_SRC_DIR}/constructor/tilemap_node.cpp
    #${NARENGINE_SRC_DIR}/render/quadrant.cpp
    #${NARENGINE_SRC_DIR}/render/qtree_render_system.cpp
    #${NARENGINE_SRC_DIR}/render/view.cpp
    #${NARENGINE_SRC_DIR}/render/frame_render_device.cpp
    #${NARENGINE_SRC_DIR}/system/dynamic_lib.cpp
    #${NARENGINE_SRC_DIR}/system/file_scene_reader.cpp
    #${NARENGINE_SRC_DIR}/system/event_manager.cpp
    #${NARENGINE_SRC_DIR}/system/listener.cpp
    #${NARENGINE_SRC_DIR}/profiles/single_thread_profile.cpp
    ${NARENGINE_SRC_DIR}/system/monitor.cpp
    ${NARENGINE_SRC_DIR}/system/window.cpp
    ${NARENGINE_SRC_DIR}/system/exceptions.cpp
)

add_library(engine ${NARENGINE_BASE_SOURCES})
target_include_directories(engine PUBLIC ${NARENGINE_BASE_INCLUDE_DIRS})
target_link_directories(engine PUBLIC ${NARENGINE_BASE_LINK_DIRS})
target_compile_definitions(engine PUBLIC ${NARENGINE_BASE_COMPILER_DEFS})
target_link_libraries(engine PUBLIC ${NARENGINE_BASE_LINK_LIBS})


if (${NARENGINE_RENDER_OPENGL} OR ${NARENGINE_RENDER_OPENGL_ES} OR ${NARENGINE_RENDER_VULKAN})
    narengine_git_dependency(STB https://github.com/nothings/stb "5736b15" "stb-src")
endif()

# Library for OpenGL render API
if (${NARENGINE_RENDER_OPENGL} OR ${NARENGINE_RENDER_OPENGL_ES})
    find_package(OpenGL REQUIRED)
    set(NARENGINE_GL_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        "${STB_INCLUDE_DIR}"
    )
    set(NARENGINE_GL_LINK_DIRS ${NARENGINE_COMMON_LINK_DIRS})
    set(NARENGINE_GL_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS})
    set(NARENGINE_GL_LINK_LIBS engine ${OPENGL_LIBRARIES})

    set_source_files_properties(${NARENGINE_SRC_DIR}/render/opengl/glad.c PROPERTIES LANGUAGE CXX)
    set(NARENGINE_GL_SOURCES
        ${NARENGINE_SRC_DIR}/render/opengl/frame_buffer_loader.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/texture_loader.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/shader_loader.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/vertex_buffer_loader.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/render_buffer_loader.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/cubemap_loader.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/render_api.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/st_render_device.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/mt_render_device.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/shader_program.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/utils.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/glad.c
    )

    add_library(engine_render_gl ${NARENGINE_GL_SOURCES})
    target_include_directories(engine_render_gl PUBLIC ${NARENGINE_GL_INCLUDE_DIRS})
    target_compile_definitions(engine_render_gl PUBLIC ${NARENGINE_GL_COMPILER_DEFS})
    target_link_directories(engine_render_gl PUBLIC ${NARENGINE_GL_LINK_DIRS})
    target_link_libraries(engine_render_gl PUBLIC ${NARENGINE_GL_LINK_LIBS})
endif() # if (${NARENGINE_RENDER_OPENGL} OR ${NARENGINE_RENDER_OPENGL_ES})


# Library for Vulkan render API
if (${NARENGINE_RENDER_VULKAN})
    set(NARENGINE_VULKAN_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS} "GLFW_INCLUDE_VULKAN")
endif()


# Testing
if (${NARENGINE_BUILD_TESTS})
    enable_testing()
    narengine_git_dependency(CATCH2 https://github.com/catchorg/Catch2.git "v3.0.1" "catch2-src/src")

    add_executable(math_test
        ${NARENGINE_SRC_DIR}/math/test/transform_matrix_test.cpp
        ${NARENGINE_SRC_DIR}/math/test/vec_test.cpp
        ${NARENGINE_SRC_DIR}/math/test/rectangle_test.cpp
        ${NARENGINE_SRC_DIR}/math/test/math_functions_test.cpp
    )
    target_include_directories(math_test PRIVATE ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories(math_test PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
    target_link_libraries(math_test PRIVATE engine_math Catch2::Catch2WithMain)
    add_test(NAME math_test COMMAND math_test)

    add_executable(render_opengl_test
        ${NARENGINE_SRC_DIR}/render/opengl/test/st_resource_manager_test.cpp
        ${NARENGINE_SRC_DIR}/render/opengl/test/mt_resource_manager_test.cpp
    )
    target_include_directories(render_opengl_test PRIVATE ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories(render_opengl_test PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
    target_link_libraries(render_opengl_test PRIVATE engine_render_gl Catch2::Catch2WithMain)
    add_test(NAME render_opengl_test COMMAND render_opengl_test)

    if (${NARENGINE_RENDER_OPENGL} OR ${NARENGINE_RENDER_OPENGL_ES})
        add_executable(opengl_st_render_api_test
            ${NARENGINE_SRC_DIR}/test/opengl_st_render_api_test.cpp
        )
        target_include_directories(opengl_st_render_api_test PRIVATE ${NARENGINE_COMMON_INCLUDE_DIRS})
        target_link_directories(opengl_st_render_api_test PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
        target_link_libraries(opengl_st_render_api_test PRIVATE engine_render_gl)
        add_test(NAME opengl_st_render_api_test COMMAND opengl_st_render_api_test)
    endif()
endif()


# Installation
install(DIRECTORY ${NARENGINE_INCLUDE_DIR}/${NAME} DESTINATION "include")
install(FILES ${PROJECT_BINARY_DIR}/${NAME}/${NAME}config.h DESTINATION "include/${NAME}")
install(DIRECTORY ${NARENGINE_COMMON_DIR}/${NAME} DESTINATION "include")
if (${NARENGINE_RENDER_OPENGL} OR ${NARENGINE_RENDER_OPENGL_ES})
    install(DIRECTORY ${NARENGINE_COMMON_DIR}/glad DESTINATION "include")
endif()
if (${WIN32})
    if (${BUILD_SHARED_LIBS})
        install(FILES ${PROJECT_BINARY_DIR}/engine.lib DESTINATION lib)
    endif()
    install(FILES ${PROJECT_BINARY_DIR}/engine.dll DESTINATION bin)
else()
    install(TARGETS engine DESTINATION lib)
endif()
