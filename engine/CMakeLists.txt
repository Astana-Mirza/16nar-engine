cmake_minimum_required(VERSION 3.14)

macro(set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR src)
set(INCLUDE_DIR include)
set(COMMON_DIR ../common)

add_library(engine
	${SRC_DIR}/game.cpp
	${SRC_DIR}/abstract/drawable.cpp
	${SRC_DIR}/abstract/signallable.cpp
	${SRC_DIR}/constructor/abstract/drawable_node.cpp
	${SRC_DIR}/constructor/abstract/node.cpp
	${SRC_DIR}/constructor/states/scene_state.cpp
	${SRC_DIR}/constructor/node_2d.cpp
	${SRC_DIR}/constructor/sprite_node.cpp
	${SRC_DIR}/constructor/world_node.cpp
	${SRC_DIR}/render/quadrant.cpp
	${SRC_DIR}/system/dynamic_lib.cpp
	${SRC_DIR}/system/file_scene_reader.cpp
)

set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

set_option(BUILD_SHARED_LIBS OFF BOOL "Build engine as static or dynamic library")

set_option(USE_SFML ON BOOL "Use SFML for rendering and controlling")
configure_file(${INCLUDE_DIR}/16narconfig.h.in 16narconfig.h)


if (USE_SFML)
	set_option(SFML_STATIC ON BOOL "Link SFML statically")
	set_option(SFML_INCLUDE_DIR . PATH "Path to SFML headers")
	set_option(SFML_LIB_DIR . PATH "Path to SFML static libraries")
	set_option(SFML_DEP_LIB_DIR . PATH "Path to SFML dependencies static libraries")

	set(EXTRA_INCLUDE_DIRS ${SFML_INCLUDE_DIR})
	set(EXTRA_LINK_DIRS ${SFML_LIB_DIR} ${SFML_DEP_LIB_DIR})

	if (${SFML_STATIC} AND ${BUILD_SHARED_LIBS})
		message(FATAL_ERROR "SFML_STATIC and BUILD_SHARED_LIBS cannot be both ON")
	endif()

	if (SFML_STATIC)
		set(EXTRA_COMPILER_DEFS SFML_STATIC)
	endif()

	if (WIN32)
		set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} opengl32.lib winmm.lib gdi32.lib)
		if (SFML_STATIC)
			if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
				set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system-s-d.lib sfml-graphics-s-d.lib sfml-window-s-d.lib)
			elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
				set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system-s.lib sfml-graphics-s.lib sfml-window-s.lib)
			endif()
		else()
			if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
				set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system-d.lib sfml-graphics-d.lib sfml-window-d.lib)
			elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
				set(EXTRA_LINK_LIBS ${EXTRA_LINK_LIBS} sfml-system.lib sfml-graphics.lib sfml-window.lib)
			endif()
		endif()
	endif() # if (WIN32)
endif() # if (USE_SFML)

target_include_directories(engine PRIVATE ${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${COMMON_DIR} ${EXTRA_INCLUDE_DIRS})
target_compile_definitions(engine PRIVATE ${EXTRA_COMPILER_DEFS})
target_link_directories(engine PRIVATE ${EXTRA_LINK_DIRS})
target_link_libraries(engine PRIVATE ${EXTRA_LINK_LIBS})

