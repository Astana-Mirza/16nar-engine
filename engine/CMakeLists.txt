set(NARENGINE_SRC_DIR "${PROJECT_SOURCE_DIR}/engine/src")
set(NARENGINE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/engine/include")
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/schemas/gen-cpp")

# Basic settings
if ("${BUILD_SHARED_LIBS}")
    if ("${WIN32}")
        set(NARENGINE_WIN_EXPORT ON)
        set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS}
            "NARENGINE_DLL"
            "GLFW_DLL"
        )
    endif() # if ("${WIN32}")
endif() # if ("${BUILD_SHARED_LIBS}")
set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS}
    "NARENGINE_LOG_LEVEL=${NARENGINE_LOG_LEVEL}"
    "GLFW_INCLUDE_NONE"
)
if ("${NARENGINE_RENDER_VULKAN}")
    set(NARENGINE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS} "GLFW_INCLUDE_VULKAN")
endif()
set(NARENGINE_COMMON_LINK_DIRS "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" "$<INSTALL_INTERFACE:lib>")
set(NARENGINE_COMMON_INCLUDE_DIRS
    "$<BUILD_INTERFACE:${NARENGINE_INCLUDE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:include>"
)
configure_file("${NARENGINE_INCLUDE_DIR}/${NAME}config.h.in" "${NAME}/${NAME}config.h")


# Logger library
set(NARENGINE_LOGGER_COMPILER_DEFS PUBLIC ${NARENGINE_COMMON_COMPILER_DEFS})
set(NARENGINE_LOGGER_INCLUDE_DIRS PUBLIC ${NARENGINE_COMMON_INCLUDE_DIRS})
set(NARENGINE_LOGGER_SOURCES
    "${NARENGINE_SRC_DIR}/logger/logger.cpp"
    "${NARENGINE_SRC_DIR}/logger/base_log_writer.cpp"
    "${NARENGINE_SRC_DIR}/logger/terminal_log_writer.cpp"
)
add_library("${NAME}_logger" "${NARENGINE_LIB_TYPE}" ${NARENGINE_LOGGER_SOURCES})
target_include_directories("${NAME}_logger" ${NARENGINE_LOGGER_INCLUDE_DIRS})
target_compile_definitions("${NAME}_logger" ${NARENGINE_LOGGER_COMPILER_DEFS})
set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_logger")


# Math library
set(NARENGINE_MATH_COMPILER_DEFS PUBLIC ${NARENGINE_COMMON_COMPILER_DEFS})
set(NARENGINE_MATH_INCLUDE_DIRS PUBLIC ${NARENGINE_COMMON_INCLUDE_DIRS})
set(NARENGINE_MATH_LINK_LIBS PUBLIC "${NAME}_logger" "glm::glm")
set(NARENGINE_MATH_SOURCES
    "${NARENGINE_SRC_DIR}/math/math_functions.cpp"
    "${NARENGINE_SRC_DIR}/math/transform_matrix.cpp"
)
add_library("${NAME}_math" "${NARENGINE_LIB_TYPE}" ${NARENGINE_MATH_SOURCES})
target_include_directories("${NAME}_math" ${NARENGINE_MATH_INCLUDE_DIRS})
target_compile_definitions("${NAME}_math" ${NARENGINE_MATH_COMPILER_DEFS})
target_link_libraries("${NAME}_math" ${NARENGINE_MATH_LINK_LIBS})
set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_math")


# Main engine library
set(NARENGINE_BASE_INCLUDE_DIRS
    PUBLIC ${NARENGINE_COMMON_INCLUDE_DIRS}
    PRIVATE "${GLFW_INCLUDE_DIR}" "${FLATBUFFERS_INCLUDE_DIR}" "${NARENGINE_GEN_INCLUDE_DIR}"
)
set(NARENGINE_BASE_LINK_LIBS
    PUBLIC "${NAME}_math"
    PRIVATE "glfw" "flatbuffers::libflatbuffers" "Threads::Threads")
set(NARENGINE_BASE_LINK_DIRS PUBLIC ${NARENGINE_COMMON_LINK_DIRS})
set(NARENGINE_BASE_COMPILER_DEFS PUBLIC ${NARENGINE_COMMON_COMPILER_DEFS})
set(NARENGINE_BASE_SOURCES
    "${NARENGINE_SRC_DIR}/game.cpp"
    "${NARENGINE_SRC_DIR}/system/monitor.cpp"
    "${NARENGINE_SRC_DIR}/system/window.cpp"
    "${NARENGINE_SRC_DIR}/system/exceptions.cpp"
    "${NARENGINE_SRC_DIR}/system/package_manager.cpp"
    "${NARENGINE_SRC_DIR}/render/camera_2d.cpp"
    "${NARENGINE_SRC_DIR}/render/drawable.cpp"
)
add_library("${NAME}_base" "${NARENGINE_LIB_TYPE}" ${NARENGINE_BASE_SOURCES})
add_dependencies("${NAME}_base" "gen-cpp" "GENERATE_gen-cpp")
target_include_directories("${NAME}_base" ${NARENGINE_BASE_INCLUDE_DIRS})
target_link_directories("${NAME}_base" ${NARENGINE_BASE_LINK_DIRS})
target_compile_definitions("${NAME}_base" ${NARENGINE_BASE_COMPILER_DEFS})
target_link_libraries("${NAME}_base" ${NARENGINE_BASE_LINK_LIBS})
set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_base")


# Library for OpenGL render API
if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")
    set(NARENGINE_GL_INCLUDE_DIRS
        PUBLIC ${NARENGINE_COMMON_INCLUDE_DIRS}
        PRIVATE ${OPENGL_INCLUDE_DIRS} "${STB_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}"
    )
    set(NARENGINE_GL_LINK_DIRS PUBLIC ${NARENGINE_COMMON_LINK_DIRS})
    set(NARENGINE_GL_COMPILER_DEFS PUBLIC ${NARENGINE_COMMON_COMPILER_DEFS})
    set(NARENGINE_GL_LINK_LIBS PUBLIC "${NAME}_base" PRIVATE "glfw" ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

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

    add_library("${NAME}_render_gl" "${NARENGINE_LIB_TYPE}" ${NARENGINE_GL_SOURCES})
    target_include_directories("${NAME}_render_gl" ${NARENGINE_GL_INCLUDE_DIRS})
    target_compile_definitions("${NAME}_render_gl" ${NARENGINE_GL_COMPILER_DEFS})
    target_link_directories("${NAME}_render_gl" ${NARENGINE_GL_LINK_DIRS})
    target_link_libraries("${NAME}_render_gl" ${NARENGINE_GL_LINK_LIBS})
    set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_render_gl")
endif() # if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")


# constructor architecture library
if ("${NARENGINE_BUILD_CONSTRUCTOR2D}")
    set(NARENGINE_CONSTRUCTOR2D_INCLUDE_DIRS
        PUBLIC ${NARENGINE_COMMON_INCLUDE_DIRS}
        PRIVATE "${FLATBUFFERS_INCLUDE_DIR}" "${NARENGINE_GEN_INCLUDE_DIR}")
    set(NARENGINE_CONSTRUCTOR2D_LINK_LIBS
        PUBLIC "${NAME}_base"
        PRIVATE "flatbuffers::libflatbuffers" "Threads::Threads")
    set(NARENGINE_CONSTRUCTOR2D_LINK_DIRS PUBLIC ${NARENGINE_COMMON_LINK_DIRS})
    set(NARENGINE_CONSTRUCTOR2D_COMPILER_DEFS PUBLIC ${NARENGINE_COMMON_COMPILER_DEFS})
    set(NARENGINE_CONSTRUCTOR2D_SOURCES
        "${NARENGINE_SRC_DIR}/constructor2d/render/drawable_2d.cpp"
        "${NARENGINE_SRC_DIR}/constructor2d/render/quadrant.cpp"
        "${NARENGINE_SRC_DIR}/constructor2d/render/qtree_render_system.cpp"
        "${NARENGINE_SRC_DIR}/constructor2d/profiles/single_thread_profile.cpp"
    )

    add_library("${NAME}_constructor2d" "${NARENGINE_LIB_TYPE}" ${NARENGINE_CONSTRUCTOR2D_SOURCES})
    target_include_directories("${NAME}_constructor2d" ${NARENGINE_CONSTRUCTOR2D_INCLUDE_DIRS})
    target_compile_definitions("${NAME}_constructor2d" ${NARENGINE_CONSTRUCTOR2D_COMPILER_DEFS})
    target_link_directories("${NAME}_constructor2d" ${NARENGINE_CONSTRUCTOR2D_LINK_DIRS})
    target_link_libraries("${NAME}_constructor2d" ${NARENGINE_CONSTRUCTOR2D_LINK_LIBS})
    set(NARENGINE_INSTALL_TARGETS ${NARENGINE_INSTALL_TARGETS} "${NAME}_constructor2d")
endif() # if ("${NARENGINE_BUILD_CONSTRUCTOR2D}")


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
    target_link_libraries("${NAME}_camera_test" PRIVATE "${NAME}_base" "Catch2::Catch2WithMain")
    add_test(NAME "${NAME}_camera_test" COMMAND "${NAME}_camera_test")

    if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")
        add_executable("${NAME}_render_opengl_test"
            "${NARENGINE_SRC_DIR}/render/opengl/test/st_resource_manager_test.cpp"
            "${NARENGINE_SRC_DIR}/render/opengl/test/mt_resource_manager_test.cpp"
        )
        target_include_directories("${NAME}_render_opengl_test" PRIVATE
            ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
        target_link_directories("${NAME}_render_opengl_test" PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
        target_link_libraries("${NAME}_render_opengl_test"
            PRIVATE "${NAME}_render_gl" "Threads::Threads" "Catch2::Catch2WithMain")
        add_test(NAME "${NAME}_render_opengl_test" COMMAND "${NAME}_render_opengl_test")
     endif() # if ("${NARENGINE_RENDER_OPENGL}" OR "${NARENGINE_RENDER_OPENGL_ES}")

     if ("${NARENGINE_BUILD_CONSTRUCTOR2D}")
          add_executable("${NAME}_constructor2d_qtree_test"
               "${NARENGINE_SRC_DIR}/constructor2d/render/test/qtree_test.cpp"
          )
          target_include_directories("${NAME}_constructor2d_qtree_test" PRIVATE
               ${NARENGINE_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
          target_link_directories("${NAME}_constructor2d_qtree_test" PRIVATE ${NARENGINE_COMMON_LINK_DIRS})
          target_link_libraries("${NAME}_constructor2d_qtree_test" PRIVATE "${NAME}_constructor2d" "Catch2::Catch2WithMain")
          add_test(NAME "${NAME}_constructor2d_qtree_test" COMMAND "${NAME}_constructor2d_qtree_test")
     endif() # if ("${NARENGINE_BUILD_CONSTRUCTOR2D}")
endif() # if ("${NARENGINE_BUILD_TESTS}")


# Installation
install(TARGETS ${NARENGINE_INSTALL_TARGETS} EXPORT "16nar")
install(DIRECTORY "${NARENGINE_INCLUDE_DIR}/${NAME}" DESTINATION "include")
install(FILES "${PROJECT_BINARY_DIR}/engine/${NAME}/${NAME}config.h" DESTINATION "include/${NAME}")
