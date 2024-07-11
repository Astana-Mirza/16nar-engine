set(NARENGINE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(NARENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/generated-includes")

# Basic settings

if ("${WIN32}" AND "${BUILD_SHARED_LIBS}")
    set(NARENGINE_WIN_EXPORT ON)
    set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS}
        "NARENGINE_DLL"
        "GLFW_DLL"
    )
endif()
set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS}
    "NARENGINE_LOG_LEVEL=${NARENGINE_LOG_LEVEL}"
    "GLFW_INCLUDE_NONE"
    "GLM_FORCE_INLINE"
    "GLM_FORCE_DEFAULT_ALIGNED_GENTYPES"
    "GLM_FORCE_INTRINSICS"
    "GLM_FORCE_SIZE_T_LENGTH"
)
if ("${NARENGINE_RENDER_VULKAN}")
    set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS} "GLFW_INCLUDE_VULKAN")
endif()
set(NARENGINE_COMMON_LINK_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
set(NARENGINE_COMMON_INCLUDE_DIRS
    "${NARENGINE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${NARENGINE_GEN_INCLUDE_DIR}"
)
configure_file("${NARENGINE_INCLUDE_DIR}/${NAME}config.h.in" "${NAME}/${NAME}config.h")


# Logger library
set(NARENGINE_LOGGER_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS})
set(NARENGINE_LOGGER_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS})
set(NARENGINE_LOGGER_SOURCES
    "${NARENGINE_SRC_DIR}/logger/logger.cpp"
    "${NARENGINE_SRC_DIR}/logger/base_log_writer.cpp"
    "${NARENGINE_SRC_DIR}/logger/terminal_log_writer.cpp"
)
add_library("${NAME}_logger" ${NARENGINE_LOGGER_SOURCES})
target_include_directories("${NAME}_logger" PUBLIC ${NARENGINE_LOGGER_INCLUDE_DIRS})
target_compile_definitions("${NAME}_logger" PUBLIC ${NARENGINE_LOGGER_COMPILER_DEFS})
set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_logger")


# Math library
set(NARENGINE_MATH_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS})
set(NARENGINE_MATH_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS} "${GLM_INCLUDE_DIR}")
set(NARENGINE_MATH_SOURCES
    "${NARENGINE_SRC_DIR}/math/math_functions.cpp"
    "${NARENGINE_SRC_DIR}/math/transform_matrix.cpp"
)
set(NARENGINE_MATH_LINK_LIBS "${NAME}_logger")
add_library("${NAME}_math" ${NARENGINE_MATH_SOURCES})
target_include_directories("${NAME}_math" PUBLIC ${NARENGINE_MATH_INCLUDE_DIRS})
target_compile_definitions("${NAME}_math" PUBLIC ${NARENGINE_MATH_COMPILER_DEFS})
target_link_libraries("${NAME}_math" PUBLIC ${NARENGINE_MATH_LINK_LIBS})
set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_math")


# Main engine library
set(NARENGINE_BASE_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS}
    "${GLFW_INCLUDE_DIR}"
    "${FLATBUFFERS_INCLUDE_DIR}"
)
set(NARENGINE_BASE_LINK_LIBS "${NAME}_math" "${NAME}_logger" "glfw")
set(NARENGINE_BASE_LINK_DIRS ${NARENGINE_COMMON_LINK_DIRS})
set(NARENGINE_BASE_SOURCES
    "${NARENGINE_SRC_DIR}/game.cpp"
    "${NARENGINE_SRC_DIR}/system/monitor.cpp"
    "${NARENGINE_SRC_DIR}/system/window.cpp"
    "${NARENGINE_SRC_DIR}/system/exceptions.cpp"
    "${NARENGINE_SRC_DIR}/system/package_manager.cpp"
    "${NARENGINE_SRC_DIR}/render/camera_2d.cpp"
)
add_library("${NAME}_base" ${NARENGINE_BASE_SOURCES})
target_include_directories("${NAME}_base" PUBLIC ${NARENGINE_BASE_INCLUDE_DIRS})
target_link_directories("${NAME}_base" PUBLIC ${NARENGINE_BASE_LINK_DIRS})
target_compile_definitions("${NAME}_base" PUBLIC ${NARENGINE_BASE_COMPILER_DEFS})
target_link_libraries("${NAME}_base" PUBLIC ${NARENGINE_BASE_LINK_LIBS})
set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_base")


# Library for OpenGL render API
if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")
    find_package(OpenGL REQUIRED)
    set(NARENGINE_GL_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        "${STB_INCLUDE_DIR}"
        "${GLFW_INCLUDE_DIR}"
    )
    set(NARENGINE_GL_LINK_DIRS ${NARENGINE_COMMON_LINK_DIRS})
    set(NARENGINE_GL_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS})
    set(NARENGINE_GL_LINK_LIBS "${NAME}_base" "${NAME}_math" "${NAME}_logger" "glfw" ${OPENGL_LIBRARIES})

    set_source_files_properties("${NARENGINE_SRC_DIR}/render/opengl/glad.c" PROPERTIES LANGUAGE CXX)
    set(NARENGINE_GL_SOURCES
        "${NARENGINE_SRC_DIR}/render/opengl/frame_buffer_loader.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/texture_loader.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/shader_loader.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/vertex_buffer_loader.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/render_buffer_loader.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/cubemap_loader.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/render_api.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/st_render_device.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/mt_render_device.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/shader_program.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/utils.cpp"
        "${NARENGINE_SRC_DIR}/render/opengl/glad.c"
    )

    add_library("${NAME}_render_gl" ${NARENGINE_GL_SOURCES})
    target_include_directories("${NAME}_render_gl" PUBLIC ${NARENGINE_GL_INCLUDE_DIRS})
    target_compile_definitions("${NAME}_render_gl" PUBLIC ${NARENGINE_GL_COMPILER_DEFS})
    target_link_directories("${NAME}_render_gl" PUBLIC ${NARENGINE_GL_LINK_DIRS})
    target_link_libraries("${NAME}_render_gl" PUBLIC ${NARENGINE_GL_LINK_LIBS})
    set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_render_gl")
endif() # if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")


# constructor architecture library
if ("${NARENGINE_BUILD_CONSTRUCTOR}")
    set(NARENGINE_CONSTRUCTOR_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS} "${FLATBUFFERS_INCLUDE_DIR}")
    set(NARENGINE_CONSTRUCTOR_LINK_LIBS "${NAME}_base" "${NAME}_math" "${NAME}_logger" glfw)
    set(NARENGINE_CONSTRUCTOR_LINK_DIRS ${NARENGINE_COMMON_LINK_DIRS})
    set(NARENGINE_CONSTRUCTOR_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS})
    set(NARENGINE_CONSTRUCTOR_SOURCES
        "${NARENGINE_SRC_DIR}/constructor/render/drawable.cpp"
        "${NARENGINE_SRC_DIR}/constructor/render/drawable_2d.cpp"
        "${NARENGINE_SRC_DIR}/constructor/render/quadrant.cpp"
        "${NARENGINE_SRC_DIR}/constructor/render/qtree_render_system.cpp"
        "${NARENGINE_SRC_DIR}/constructor/profiles/single_thread_profile.cpp"
    )

    add_library("${NAME}_constructor" ${NARENGINE_CONSTRUCTOR_SOURCES})
    target_include_directories("${NAME}_constructor" PUBLIC ${NARENGINE_CONSTRUCTOR_INCLUDE_DIRS})
    target_compile_definitions("${NAME}_constructor" PUBLIC ${NARENGINE_CONSTRUCTOR_COMPILER_DEFS})
    target_link_directories("${NAME}_constructor" PUBLIC ${NARENGINE_CONSTRUCTOR_LINK_DIRS})
    target_link_libraries("${NAME}_constructor" PUBLIC ${NARENGINE_CONSTRUCTOR_LINK_LIBS})
    set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_constructor")
endif() # if ("${NARENGINE_BUILD_CONSTRUCTOR}")


# Testing
if ("${NARENGINE_BUILD_TESTS}")
    enable_testing()

    add_executable("${NAME}_math_test"
        "${NARENGINE_SRC_DIR}/math/test/transform_matrix_test.cpp"
        "${NARENGINE_SRC_DIR}/math/test/vec_test.cpp"
        "${NARENGINE_SRC_DIR}/math/test/rectangle_test.cpp"
        "${NARENGINE_SRC_DIR}/math/test/math_functions_test.cpp"
    )
    target_include_directories("${NAME}_math_test" PRIVATE ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories("${NAME}_math_test" PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
    target_link_libraries("${NAME}_math_test" PRIVATE "${NAME}_math" "${NAME}_logger" "Catch2::Catch2WithMain")
    add_test(NAME "${NAME}_math_test" COMMAND "${NAME}_math_test")

    add_executable("${NAME}_camera_test"
        "${NARENGINE_SRC_DIR}/render/test/camera_2d_test.cpp"
    )
    target_include_directories("${NAME}_camera_test" PRIVATE ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories("${NAME}_camera_test" PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
    target_link_libraries("${NAME}_camera_test" PRIVATE
        "${NAME}_base" "${NAME}_math" "${NAME}_logger" "Catch2::Catch2WithMain")
    add_test(NAME "${NAME}_camera_test" COMMAND "${NAME}_camera_test")

    if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")
        add_executable("${NAME}_render_opengl_test"
            "${NARENGINE_SRC_DIR}/render/opengl/test/st_resource_manager_test.cpp"
            "${NARENGINE_SRC_DIR}/render/opengl/test/mt_resource_manager_test.cpp"
        )
        target_include_directories("${NAME}_render_opengl_test" PRIVATE
            ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
        target_link_directories("${NAME}_render_opengl_test" PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
        target_link_libraries("${NAME}_render_opengl_test" PRIVATE
            "${NAME}_render_gl" "${NAME}_math" "${NAME}_logger" "glfw" "Catch2::Catch2WithMain")
        add_test(NAME "${NAME}_render_opengl_test" COMMAND "${NAME}_render_opengl_test")

        if ("${NARENGINE_BUILD_CONSTRUCTOR}")
            add_executable("${NAME}_opengl_st_api_test"
                "${NARENGINE_SRC_DIR}/constructor/test/opengl_st_render_api_test.cpp"
            )
            target_include_directories("${NAME}_opengl_st_api_test" PRIVATE ${NARENGINE_COMMON_INCLUDE_DIRS})
            target_link_directories("${NAME}_opengl_st_api_test" PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
            target_link_libraries("${NAME}_opengl_st_api_test" PRIVATE
                "${NAME}_constructor" "${NAME}_render_gl" "${NAME}_base" "${NAME}_math" "${NAME}_logger" "glfw")
            add_test(NAME "${NAME}_opengl_st_api_test" COMMAND "${NAME}_opengl_st_api_test")
        endif()
     endif() # if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")
endif() # if ("${NARENGINE_BUILD_TESTS}")


# Installation
install(DIRECTORY "${NARENGINE_INCLUDE_DIR}/${NAME}" DESTINATION "include")
install(FILES "${PROJECT_BINARY_DIR}/${NAME}/${NAME}config.h" DESTINATION "include/${NAME}")
if ("${WIN32}")
    list(TRANSFORM "NARENGINE_INSTALL_TARGETS" PREPEND "${PROJECT_BINARY_DIR}"
        OUTPUT_VARIABLE "NARENGINE_INSTALL_TARGETS_FULL")
    if ("${BUILD_SHARED_LIBS}")
        list(TRANSFORM "NARENGINE_INSTALL_TARGETS_FULL" APPEND ".lib"
            OUTPUT_VARIABLE "NARENGINE_INSTALL_TARGETS_LIB")
        install(FILES ${NARENGINE_INSTALL_TARGETS_LIB} DESTINATION "lib")
    endif()
    list(TRANSFORM "NARENGINE_INSTALL_TARGETS_FULL" APPEND ".bin"
        OUTPUT_VARIABLE "NARENGINE_INSTALL_TARGETS_BIN")
    install(FILES ${NARENGINE_INSTALL_TARGETS_LIB} DESTINATION "bin")
else()
    install(TARGETS ${NARENGINE_INSTALL_TARGETS} DESTINATION "lib")
endif()
