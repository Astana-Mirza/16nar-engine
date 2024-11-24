cmake_minimum_required(VERSION 3.15)

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


project(16nar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

# Wayland needs these packages: libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules
# X11 needs xorg-dev
narengine_set_option(NARENGINE_RENDER_OPENGL ON BOOL "Build with OpenGL support (uses GLM + GLFW + glad)")
#narengine_set_option(NARENGINE_RENDER_VULKAN ON BOOL "Build with Vulkan support (uses GLM + GLFW)") # not implemented
#narengine_set_option(NARENGINE_RENDER_OPENGL_ES ON BOOL "Build with OpenGL ES support (uses GLM + GLFW + glad)") # not implemented

narengine_set_option(NARENGINE_TOOLS_FLATBUFFERS ON BOOL "Build tools for using flatbuffers format")
narengine_set_option(NARENGINE_TOOLS_JSON ON BOOL "Build tools for using flatbuffers format")

narengine_set_option(NARENGINE_BUILD_UTILS ON BOOL "Build utilities for development with engine")
narengine_set_option(NARENGINE_LOG_LEVEL "9" STRING "Maximum log level of engine logging library")
narengine_set_option(NARENGINE_BUILD_TESTS "${BUILD_TESTING}" BOOL "Build tests for engine")

narengine_set_option(NARENGINE_BUILD_CONSTRUCTOR2D ON BOOL "Build constructor2d architecture")

# Dependencies
# Visual Studio 17 2022 bug workaround
if ("${GENERATOR_IS_MULTI_CONFIG}" OR ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 17 2022"))
    set(NARENGINE_GENERATORS_PATH "${PROJECT_BINARY_DIR}/generators")
    message("BUILD GENERATORS DIR ${NARENGINE_GENERATORS_PATH} (MULTI-CONFIG, ${CMAKE_GENERATOR})")
else()
    set(NARENGINE_GENERATORS_PATH "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators")
    message("BUILD GENERATORS DIR ${NARENGINE_GENERATORS_PATH} (SINGLE-CONFIG, ${CMAKE_GENERATOR})")
endif() # if ("${GENERATOR_IS_MULTI_CONFIG}")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${NARENGINE_GENERATORS_PATH}")
set(CMAKE_TOOLCHAIN_FILE "${NARENGINE_GENERATORS_PATH}/conan_toolchain.cmake")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
if ("${NARENGINE_RENDER_OPENGL}")
    find_package(OpenGL REQUIRED)
endif()
if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    find_package(flatbuffers CONFIG REQUIRED)
endif()
if ("${NARENGINE_TOOLS_JSON}")
    find_package(nlohmann_json CONFIG REQUIRED)
    find_package(STB CONFIG REQUIRED)
endif()
if ("${NARENGINE_BUILD_TESTS}")
    find_package(Catch2 CONFIG REQUIRED)
    enable_testing()
endif()


# Common settings
set(NAME 16nar)
if("${BUILD_SHARED_LIBS}")
    set(NARENGINE_LIB_TYPE "SHARED")
else()
    set(NARENGINE_LIB_TYPE "STATIC")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/schemas")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine")

install(EXPORT "${NAME}" DESTINATION "lib/cmake" NAMESPACE "${NAME}::")
