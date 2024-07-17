cmake_minimum_required(VERSION 3.15)
include(FetchContent)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,\
        RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
endif()

if (POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()


macro(narengine_set_option var default opt_type docstring)
    if(NOT DEFINED "${var}")
        set("${var}" ${default})
    endif()
    set("${var}" ${${var}} CACHE "${opt_type}" "${docstring}" FORCE)
endmacro()

macro(narengine_git_dependency dep_name git_repo git_tag incl_dir)
    FetchContent_Declare(
        "${dep_name}"
        GIT_REPOSITORY "${git_repo}"
        GIT_TAG        "${git_tag}"
    )
    set("${dep_name}_INCLUDE_DIR" "${FETCHCONTENT_BASE_DIR}/${incl_dir}")
    FetchContent_MakeAvailable("${dep_name}")
endmacro()


project(16nar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
narengine_set_option(NARENGINE_BUILD_TOOLS ON BOOL "Build tools for development with engine")
narengine_set_option(NARENGINE_BUILD_TESTS OFF BOOL "Build tests for engine")

narengine_set_option(NARENGINE_BUILD_CONSTRUCTOR ON BOOL "Build constructor architecture")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# GLM dependency
narengine_git_dependency(GLM https://github.com/g-truc/glm "bf71a83" "glm-src")

# GLFW dependency
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
set(GLFW_USE_WAYLAND "${NARENGINE_WINDOW_WAYLAND}")
narengine_git_dependency(GLFW https://github.com/glfw/glfw "3.3.8" "glfw-src/include")

# FlatBuffers dependency
set(FLATBUFFERS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
narengine_git_dependency(FLATBUFFERS https://github.com/google/flatbuffers "v24.3.25" "flatbuffers-src/include")

if ("${NARENGINE_BUILD_TOOLS}")
    # nlohmann::json dependency
    narengine_git_dependency(nlohmann_json "https://github.com/nlohmann/json.git"
        "v3.11.2" "nlohmann_json-src/single_include")

    # STB dependency
    if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}" OR "${NARENGINE_RENDER_VULKAN}")
        narengine_git_dependency(STB https://github.com/nothings/stb "5736b15" "stb-src")
    endif()
endif()

# Catch2 dependency
if ("${NARENGINE_BUILD_TESTS}")
    narengine_git_dependency(CATCH2 https://github.com/catchorg/Catch2.git "v3.0.1" "catch2-src/src")
    enable_testing()
endif()

set(NAME 16nar)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/schemas")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")
if ("${NARENGINE_BUILD_TOOLS}")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tools")
endif()
