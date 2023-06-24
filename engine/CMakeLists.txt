cmake_minimum_required(VERSION 3.14)
include(FetchContent)

macro(set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(BUILD_SHARED_LIBS ON)

set(NAME 16nar)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common)

set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")
set_option(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" STRING "Choose the install directory")
set_option(USE_SFML ON BOOL "Use SFML for rendering and controlling")


if (WIN32)
    set(WIN_EXPORT ON)
    set(EXTRA_COMPILER_DEFS ${EXTRA_COMPILER_DEFS} BUILDING_DLL)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

add_library(engine
    ${SRC_DIR}/game.cpp
    ${SRC_DIR}/abstract/signallable.cpp
    ${SRC_DIR}/constructor/abstract/drawable_node.cpp
    ${SRC_DIR}/constructor/abstract/node.cpp
    ${SRC_DIR}/constructor/states/scene_state.cpp
    ${SRC_DIR}/constructor/tiles/tile.cpp
    ${SRC_DIR}/constructor/animations/animation.cpp
    ${SRC_DIR}/constructor/node_2d.cpp
    ${SRC_DIR}/constructor/sprite_node.cpp
    ${SRC_DIR}/constructor/sound_node.cpp
    ${SRC_DIR}/constructor/text_node.cpp
    ${SRC_DIR}/constructor/world_node.cpp
    ${SRC_DIR}/constructor/tilemap_node.cpp
    ${SRC_DIR}/render/quadrant.cpp
    ${SRC_DIR}/render/qtree_render_system.cpp
    ${SRC_DIR}/render/view.cpp
    ${SRC_DIR}/render/frame_render_device.cpp
    ${SRC_DIR}/system/dynamic_lib.cpp
    ${SRC_DIR}/system/file_scene_reader.cpp
    ${SRC_DIR}/system/event_manager.cpp
    ${SRC_DIR}/system/listener.cpp
    ${SRC_DIR}/system/profiles/single_thread_profile.cpp
)

if (USE_SFML)
    set(SFML_INSTALL_PKGCONFIG_FILES OFF)

    # On linux requires libudev-dev, libopenal-dev, libvorbis-dev, libflac-dev
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG        "2.6.x"
    )

    set(SFML_INCLUDE_DIR "${FETCHCONTENT_BASE_DIR}/sfml-src/include")
    set(SFML_LIB_DIR "${FETCHCONTENT_BASE_DIR}/sfml-build/lib")
    if (WIN32)
        set(SFML_DEP_LIB_DIR "${FETCHCONTENT_BASE_DIR}/sfml-src/extlibs/libs-msvc/x64")
    endif()

    set(EXTRA_INCLUDE_DIRS ${SFML_INCLUDE_DIR})
    set(EXTRA_LINK_DIRS ${SFML_LIB_DIR} ${SFML_DEP_LIB_DIR})
    set(EXTRA_DEPENDENCIES sfml-system sfml-graphics sfml-window sfml-audio)

    if (WIN32)
        set(EXTRA_DEPENDENCIES ${EXTRA_DEPENDENCIES} sfml-main)
        set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} opengl32 openal32 winmm gdi32 freetype vorbis vorbisenc vorbisfile ogg flac)
        if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
            set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system-d sfml-graphics-d sfml-window-d sfml-audio-d)
        elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
            set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system sfml-graphics sfml-window sfml-audio)
        endif()
    else()
        include_directories(/usr/include/freetype2)
        set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system sfml-graphics sfml-window sfml-audio)
    endif() # if (WIN32)

    add_dependencies(engine ${EXTRA_DEPENDENCIES})
    FetchContent_MakeAvailable(SFML)
endif() # if (USE_SFML)

configure_file(${INCLUDE_DIR}/${NAME}config.h.in ${NAME}/${NAME}config.h)

target_include_directories(engine PRIVATE ${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${COMMON_DIR} ${EXTRA_INCLUDE_DIRS})
target_compile_definitions(engine PRIVATE ${EXTRA_COMPILER_DEFS})
target_link_directories(engine PRIVATE ${EXTRA_LINK_DIRS})
target_link_libraries(engine PRIVATE ${EXTRA_LINK_LIBS})

install(DIRECTORY ${INCLUDE_DIR}/${NAME} DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/${NAME}/${NAME}config.h DESTINATION include/${NAME})
install(FILES ${COMMON_DIR}/${NAME}/scene_file.h DESTINATION include/${NAME})
if (WIN32)
    install(FILES ${PROJECT_BINARY_DIR}/engine.lib DESTINATION lib)
    install(FILES ${PROJECT_BINARY_DIR}/engine.dll DESTINATION bin)
else()
    install(TARGETS engine DESTINATION lib)
endif()
