cmake_minimum_required(VERSION 3.22)

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

project(16nar VERSION "${NARENGINE_VERSION_MAJOR}.${NARENGINE_VERSION_MINOR}.${NARENGINE_VERSION_PATCH}" LANGUAGES CXX)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/NarengineDefs.cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wayland needs these packages: libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules
# X11 needs xorg-dev
narengine_set_option(NARENGINE_RENDER_VULKAN ON BOOL "Build with Vulkan support")
narengine_set_option(NARENGINE_RENDER_OPENGL OFF BOOL "Build with OpenGL support") # not implemented
#narengine_set_option(NARENGINE_RENDER_OPENGL_ES ON BOOL "Build with OpenGL ES support") # not implemented

narengine_set_option(NARENGINE_BUILD_UTILS ON BOOL "Build utilities for development with engine")
narengine_set_option(NARENGINE_LOG_LEVEL "9" STRING "Maximum log level of engine logging library")
narengine_set_option(NARENGINE_BUILD_TESTS "${PROJECT_IS_TOP_LEVEL}" BOOL "Build tests for engine")
narengine_set_option(NARENGINE_BUILD_DOCS "${PROJECT_IS_TOP_LEVEL}" BOOL "Build documentation for engine")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(NARENGINE_DEBUG_BUILD ON)
else()
    set(NARENGINE_DEBUG_BUILD OFF)
endif()

# Dependencies
set(NARENGINE_GENERATORS_PATH "${PROJECT_BINARY_DIR}/generators")
if ("${GENERATOR_IS_MULTI_CONFIG}")
    message("BUILD GENERATORS DIR ${NARENGINE_GENERATORS_PATH} (MULTI-CONFIG, ${CMAKE_GENERATOR})")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_"${CMAKE_BUILD_TYPE}" "${PROJECT_BINARY_DIR}/bin")
else()
    message("BUILD GENERATORS DIR ${NARENGINE_GENERATORS_PATH} (SINGLE-CONFIG, ${CMAKE_GENERATOR})")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif() # if ("${GENERATOR_IS_MULTI_CONFIG}")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${NARENGINE_GENERATORS_PATH}")
set(CMAKE_TOOLCHAIN_FILE "${NARENGINE_GENERATORS_PATH}/conan_toolchain.cmake")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(flatbuffers CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(STB CONFIG REQUIRED)
find_package(inih CONFIG REQUIRED)
if ("${NARENGINE_RENDER_OPENGL}")
    find_package(OpenGL REQUIRED)
endif()
if ("${NARENGINE_BUILD_UTILS}")
    find_package(cxxopts CONFIG REQUIRED)
endif()
if ("${NARENGINE_BUILD_TESTS}")
    find_package(Catch2 CONFIG REQUIRED)
    set(BUILD_TESTING ON)
    include(CTest)
endif()
if("${NARENGINE_BUILD_DOCS}")
    find_package(Doxygen REQUIRED)
endif()


# Common settings
set(NAME 16nar)
if("${BUILD_SHARED_LIBS}")
    if ("${WIN32}")
        # for __declspec( dllexport ) or __declspec( dllimport )
        set(NARENGINE_WIN_EXPORT ON)
    endif()
    set(NARENGINE_LIB_TYPE "SHARED")
else()
    set(NARENGINE_LIB_TYPE "STATIC")
endif()
set(NARENGINE_COMMON_COMPILER_DEFS "")
set(NARENGINE_COMMON_INCLUDE_DIRS "")
set(NARENGINE_COMMON_LINK_DIRS "")
set(NARENGINE_COMMON_LINK_LIBS "")


add_subdirectory("${PROJECT_SOURCE_DIR}/schemas")
add_subdirectory("${PROJECT_SOURCE_DIR}/platform")
add_subdirectory("${PROJECT_SOURCE_DIR}/core")
add_subdirectory("${PROJECT_SOURCE_DIR}/plugin")
if("${NARENGINE_BUILD_DOCS}")
    add_subdirectory("${PROJECT_SOURCE_DIR}/docs")
endif()

install(EXPORT "${NAME}" DESTINATION "lib/cmake" NAMESPACE "${NAME}::")
