set(NARENGINE_TOOLS_SRC_DIR "${PROJECT_SOURCE_DIR}/tools/src")
set(NARENGINE_TOOLS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/tools/include")

# Basic settings
if ("${BUILD_SHARED_LIBS}" AND "${WIN32}")
    set(NARENGINE_TOOLS_COMMON_COMPILER_DEFS ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS}
        "NARENGINE_DLL"
    )
endif() # if ("${BUILD_SHARED_LIBS}" AND "${WIN32}")
set(NARENGINE_TOOLS_COMMON_INCLUDE_DIRS
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/engine/include>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tools/include>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:include>"
)
set(NARENGINE_TOOLS_COMMON_LINK_DIRS "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" "$<INSTALL_INTERFACE:lib>")


# Base tools library
set(NARENGINE_TOOLS_COMPILER_DEFS PRIVATE ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS})
set(NARENGINE_TOOLS_INCLUDE_DIRS
    PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS}
    PRIVATE "${PROJECT_BINARY_DIR}/schemas/flatbuffers-gen-cpp"
)
set(NARENGINE_TOOLS_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
set(NARENGINE_TOOLS_LINK_LIBS PRIVATE "stb::stb" "flatbuffers::libflatbuffers")
set(NARENGINE_TOOLS_SOURCES
    "${NARENGINE_TOOLS_SRC_DIR}/misc/file.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/misc/shared_buffer_ptr.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/misc/asset_iterator.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/misc/name_table.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/misc/stb_impl.cpp"

    "${NARENGINE_TOOLS_SRC_DIR}/logger/logger.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/logger/terminal_log_writer.cpp"

    "${NARENGINE_TOOLS_SRC_DIR}/resources/resource_pkg_visitor.cpp"

    "${NARENGINE_TOOLS_SRC_DIR}/memory_domain.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/memory_manager.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/unified_storage.cpp"
)
add_library("${NAME}_tools" "${NARENGINE_LIB_TYPE}" ${NARENGINE_TOOLS_SOURCES})
add_dependencies("${NAME}_tools" "flatbuffers-gen-cpp" "GENERATE_flatbuffers-gen-cpp")
target_include_directories("${NAME}_tools" ${NARENGINE_TOOLS_INCLUDE_DIRS})
target_link_directories("${NAME}_tools" ${NARENGINE_TOOLS_LINK_DIRS})
target_compile_definitions("${NAME}_tools" ${NARENGINE_TOOLS_COMPILER_DEFS})
target_link_libraries("${NAME}_tools" ${NARENGINE_TOOLS_LINK_LIBS})
set(NARENGINE_TOOLS_LIBS ${NARENGINE_TOOLS_LIBS} "${NAME}_tools")


if ("${NARENGINE_TOOLS_JSON}")
    # JSON tools library
    set(NARENGINE_TOOLS_JSON_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    set(NARENGINE_TOOLS_JSON_INCLUDE_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS})
    set(NARENGINE_TOOLS_JSON_LINK_LIBS PUBLIC "${NAME}_tools" "nlohmann_json::nlohmann_json")
    set(NARENGINE_TOOLS_JSON_COMPILER_DEFS PRIVATE ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS})
    set(NARENGINE_TOOLS_JSON_SOURCES
        "${NARENGINE_TOOLS_SRC_DIR}/json/json_asset_reader.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/json/json_asset_writer.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/json/json_asset_file_processor.cpp"
    )
    if ("${NARENGINE_TOOLS_FLATBUFFERS}")
        set(NARENGINE_TOOLS_JSON_LINK_LIBS ${NARENGINE_TOOLS_JSON_LINK_LIBS}
            PUBLIC "flatbuffers::libflatbuffers"
        )
        set(NARENGINE_TOOLS_JSON_SOURCES ${NARENGINE_TOOLS_JSON_SOURCES}
            "${NARENGINE_TOOLS_SRC_DIR}/json/json_to_flatbuffers_convertor.cpp"
        )
    endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")

    add_library("${NAME}_tools_json" "${NARENGINE_LIB_TYPE}" ${NARENGINE_TOOLS_JSON_SOURCES})
    target_include_directories("${NAME}_tools_json" ${NARENGINE_TOOLS_JSON_INCLUDE_DIRS})
    target_link_directories("${NAME}_tools_json" ${NARENGINE_TOOLS_JSON_LINK_DIRS})
    target_compile_definitions("${NAME}_tools_json" ${NARENGINE_TOOLS_JSON_COMPILER_DEFS})
    target_link_libraries("${NAME}_tools_json" ${NARENGINE_TOOLS_JSON_LINK_LIBS})
    set(NARENGINE_TOOLS_LIBS ${NARENGINE_TOOLS_LIBS} "${NAME}_tools_json")
endif() # if ("${NARENGINE_TOOLS_JSON}")


if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    # flatbuffers tools library
    set(NARENGINE_TOOLS_FB_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    set(NARENGINE_TOOLS_FB_INCLUDE_DIRS
        PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS}
        PRIVATE "${PROJECT_BINARY_DIR}/schemas/flatbuffers-gen-cpp"
    )
    set(NARENGINE_TOOLS_FB_COMPILER_DEFS PUBLIC ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS}
        "FLATBUFFERS_LOCALE_INDEPENDENT=0")
    set(NARENGINE_TOOLS_FB_LINK_LIBS PUBLIC "${NAME}_tools" "flatbuffers::libflatbuffers")
    set(NARENGINE_TOOLS_FB_SOURCES
        "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers/flatbuffers_asset_reader.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers/flatbuffers_asset_writer.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers/flatbuffers_pmr_allocator.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers/flatbuffers_asset_file_processor.cpp"
    )
    add_library("${NAME}_tools_fb" "${NARENGINE_LIB_TYPE}" ${NARENGINE_TOOLS_FB_SOURCES})
    add_dependencies("${NAME}_tools_fb" "flatbuffers-gen-cpp" "GENERATE_flatbuffers-gen-cpp")
    target_include_directories("${NAME}_tools_fb" ${NARENGINE_TOOLS_FB_INCLUDE_DIRS})
    target_link_directories("${NAME}_tools_fb" ${NARENGINE_TOOLS_FB_LINK_DIRS})
    target_compile_definitions("${NAME}_tools_fb" ${NARENGINE_TOOLS_FB_COMPILER_DEFS})
    target_link_libraries("${NAME}_tools_fb" ${NARENGINE_TOOLS_FB_LINK_LIBS})
    set(NARENGINE_TOOLS_LIBS ${NARENGINE_TOOLS_LIBS} "${NAME}_tools_fb")
endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")


if ("${NARENGINE_BUILD_CONSTRUCTOR2D}")
    # constructor2d tools library
    set(NARENGINE_TOOLS_CTR2D_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    set(NARENGINE_TOOLS_CTR2D_INCLUDE_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS})
    set(NARENGINE_TOOLS_CTR2D_COMPILER_DEFS PUBLIC ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS})
    set(NARENGINE_TOOLS_CTR2D_LINK_LIBS PUBLIC "${NAME}_tools")
    set(NARENGINE_TOOLS_CTR2D_SOURCES
        "${NARENGINE_TOOLS_SRC_DIR}/constructor2d/constructor2d_visitor.cpp"
    )
    add_library("${NAME}_tools_constructor2d" "${NARENGINE_LIB_TYPE}" ${NARENGINE_TOOLS_CTR2D_SOURCES})
    target_include_directories("${NAME}_tools_constructor2d" ${NARENGINE_TOOLS_CTR2D_INCLUDE_DIRS})
    target_link_directories("${NAME}_tools_constructor2d" ${NARENGINE_TOOLS_CTR2D_LINK_DIRS})
    target_compile_definitions("${NAME}_tools_constructor2d" ${NARENGINE_TOOLS_CTR2D_COMPILER_DEFS})
    target_link_libraries("${NAME}_tools_constructor2d" ${NARENGINE_TOOLS_CTR2D_LINK_LIBS})
    set(NARENGINE_TOOLS_LIBS ${NARENGINE_TOOLS_LIBS} "${NAME}_tools_constructor2d")
endif() # if ("${NARENGINE_BUILD_CONSTRUCTOR2D}")


set(NARENGINE_TOOLS_ALL_TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} ${NARENGINE_TOOLS_LIBS})
if ("${NARENGINE_BUILD_UTILS}")
    add_executable("${NAME}_asset_tool" "${NARENGINE_TOOLS_SRC_DIR}/utils/asset_tool.cpp")
    target_include_directories("${NAME}_asset_tool" PRIVATE
        ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS} "${NARENGINE_TOOLS_INCLUDE_DIR}")
    target_link_directories("${NAME}_asset_tool" PRIVATE ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    target_link_libraries("${NAME}_asset_tool" PRIVATE "cxxopts::cxxopts" PUBLIC ${NARENGINE_TOOLS_LIBS})
    set(NARENGINE_TOOLS_ALL_TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} "${NAME}_asset_tool")
endif() # if ("${NARENGINE_BUILD_UTILS}")


# Testing
if ("${NARENGINE_BUILD_TESTS}")
    enable_testing()
    if ("${NARENGINE_TOOLS_JSON}")
        set(NARENGINE_TOOLS_TEST_SOURCES ${NARENGINE_TOOLS_TEST_SOURCES}
            "${NARENGINE_TOOLS_SRC_DIR}/test/json_tools_test.cpp"
        )
        if ("${NARENGINE_TOOLS_FLATBUFFERS}")
            set(NARENGINE_TOOLS_TEST_SOURCES ${NARENGINE_TOOLS_TEST_SOURCES}
                "${NARENGINE_TOOLS_SRC_DIR}/test/json_to_flatbuffers_convertor_test.cpp"
            )
        endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    endif() # if ("${NARENGINE_TOOLS_JSON}")
    if ("${NARENGINE_TOOLS_FLATBUFFERS}")
        set(NARENGINE_TOOLS_TEST_SOURCES ${NARENGINE_TOOLS_TEST_SOURCES}
            # flatbuffers tests
        )
    endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    add_executable("${NAME}_tools_test" ${NARENGINE_TOOLS_TEST_SOURCES})
    target_include_directories("${NAME}_tools_test" PRIVATE
        "${NARENGINE_TOOLS_INCLUDE_DIR}" ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories("${NAME}_tools_test" PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries("${NAME}_tools_test" PRIVATE ${NARENGINE_TOOLS_LIBS} "Catch2::Catch2WithMain")
    file(COPY "${NARENGINE_TOOLS_SRC_DIR}/test/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME "${NAME}_tools_test" COMMAND "${NAME}_tools_test")
endif() # if ("${NARENGINE_BUILD_TESTS}")


# Installation
install(TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} EXPORT "16nar")
install(DIRECTORY "${NARENGINE_TOOLS_INCLUDE_DIR}/${NAME}" DESTINATION "include")
