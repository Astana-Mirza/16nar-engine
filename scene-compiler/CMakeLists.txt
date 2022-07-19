cmake_minimum_required(VERSION 3.14)

project(scene-compiler LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools)

set(SRC_DIR src)
set(TS_DIR translations)
set(INCLUDE_DIR include)
set(COMMON_DIR ../common)
set(TS_FILES ${TS_DIR}/scene-compiler_ru_RU.ts)

add_executable(scene-compiler
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/compiler.cpp
  ${INCLUDE_DIR}/compiler.h
  ${TS_FILES}
)

target_include_directories(scene-compiler PRIVATE ${INCLUDE_DIR} ${COMMON_DIR})

target_link_libraries(scene-compiler Qt${QT_VERSION_MAJOR}::Core)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
